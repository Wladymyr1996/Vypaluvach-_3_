/*
*	"Vypaluvach" - is control program for CNC wood burner "CNC Vypaluvach"
*	Copyright (C) 2019 Volodymyr Stadnyk
*	e-mail: Wladymyr1996@gmail.com
*	This program is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*	You should have received a copy of the GNU General Public License
*	along with this program. If not, see <https://www.gnu.org/licenses/>
*/

#include "commands.h"

QColor TCommands::getColorFromPause(uint16_t pause, int colorCount) {
	QColor Uber;
	qreal Kyrylo = 255.00f / (qreal)colorCount;
	Kyrylo *= pause;
	if (Kyrylo > 255) Kyrylo = 255;
	Kyrylo = 255 - Kyrylo;
	Uber.setRgb(Kyrylo, Kyrylo, Kyrylo);
	return Uber;
}

TCommands::TCommands(QObject *parent) : QObject(parent) {
	commandList = new QList<TCommand>;
	image = new QPixmap;
}

void TCommands::addCommand(uint8_t command, uint16_t parameter1, uint16_t paramerter2) {
	commandList->push_back(getCommand(command, parameter1, paramerter2));
}

TCommands::TCommand TCommands::getCommand(uint8_t command, uint16_t parameter1, uint16_t paramerter2) {
	TCommand com;
	com.command = command;
	com.parameter1 = parameter1;
	com.parameter2 = paramerter2;
	return com;
}

TCommands::TCommand TCommands::getCommand(QByteArray data) {
	TCommand command;
	command.command = data.at(0);
	command.parameter1 = ((uint8_t)data.at(1) << 8) | (uint8_t)data.at(2);
	command.parameter2 = ((uint8_t)data.at(3) << 8) | (uint8_t)data.at(4);
	return command;
}

QByteArray TCommands::getByteArrayFromCommand(TCommands::TCommand command) {
	char c[5];
	c[0] = command.command;
	c[1] = command.parameter1 >> 8;
	c[2] = command.parameter1;
	c[3] = command.parameter2 >> 8;
	c[4] = command.parameter2;

	return QByteArray(c, 5);
}


void TCommands::generateImage() {
	uint64_t timer = 0;
	int i;
	int colorCount = glData->getMaxPause() - glData->getMinPause();
	int minPause = glData->getMinPause();
	int skippedRows = glData->getSkippedRows();
	QSize imgSize;

	TCommand command;
	i = 0;
	do {
		command = commandList->at(i);
		switch (command.command) {
			case TCommandType::setColorCount:
				colorCount = command.parameter1;
				break;
			case TCommandType::setMinPauseX:
				minPause = command.parameter1;
				break;
			case TCommandType::setSkippiedRows:
				skippedRows = command.parameter1;
				break;
			case TCommandType::setImageSize:
				imgSize = QSize(command.parameter1, command.parameter2);
				break;
			case TCommandType::pause:
				timer += command.parameter1 * 1000;
				break;
		}
		i++;
	} while (command.command != TCommandType::beginPrint);

	*image = QPixmap(imgSize);
	QPainter painter(image);

	int oldX = 0, newX;
	int y = 0;
	i = 0;
	do {
		command = commandList->at(i);
		switch (command.command) {
			case TCommandType::lineTo:
				newX = command.parameter1;
				painter.setPen(QPen(getColorFromPause(command.parameter2 - minPause, colorCount)));
				timer += (newX - oldX) * command.parameter2;
				for (int j = 0; j < 1 + skippedRows; j++)
					painter.drawLine(oldX, y + j, newX, y + j);
				oldX = newX;
				break;
			case TCommandType::newRow:
				timer += minPause * (imgSize.width() + 200);
				y += 1 + skippedRows;
				oldX = 0;
				break;
		}
		i++;
	} while (command.command != endPrint);

	printingTime = timer;
}

uint16_t TCommands::getPauseFromColor(QColor color) {
	int r, g, b;
	color.getRgb(&r, &g, &b);
	int pause = r + g + b;
	pause /= 765 / (glData->getMaxPause() - glData->getMinPause());
	pause = glData->getMaxPause() - pause;
	return pause;
}

void TCommands::doMakeLinesFromImage(QString filename) {
	QImage img(filename);
	if (glData->getDoImageResize()) {
		QSize sizeTo(glData->getImageWidthPlace(), glData->getImageHeightPlace());

		if ((img.width()) * sizeTo.height() > img.height() * sizeTo.width()) {
			img = img.scaledToWidth(sizeTo.width());
		} else img = img.scaledToHeight(sizeTo.height());
	}

	if (!commandList->isEmpty()) commandList->clear();
	QSize imgSize = img.size();
	QPixmap canvas(imgSize.width() + glData->getHeatigPlaceWidth(), imgSize.height());
	QPainter painter(&canvas);
	painter.setPen(QPen(QColor(Qt::black)));
	painter.setBrush(QBrush(QColor(Qt::black)));
	painter.drawRect(0, 0, glData->getHeatigPlaceWidth(), imgSize.height());
	painter.drawImage(glData->getHeatigPlaceWidth(), 0, img);
	painter.end();

	img = canvas.toImage();

	int beginPoint, endPoint;
	int currentPause, nextPause;

	addCommand(TCommandType::setColorCount, glData->getMaxPause() - glData->getMinPause());
	addCommand(TCommandType::setMinPauseX, glData->getMinPause());
	addCommand(TCommandType::setSkippiedRows, glData->getSkippedRows());
	addCommand(TCommandType::setSkippiedYSteps, glData->getSkippedYStep());
	addCommand(TCommandType::setImageSize, img.width(), img.height());
	addCommand(TCommandType::setAsZero);
	addCommand(TCommandType::beginPrint);
	addCommand(TCommandType::pause, glData->getHeatingTime());

	for (int y = 0; y < img.height(); y += 1 + glData->getSkippedRows()) {
		beginPoint = 0;
		currentPause = getPauseFromColor(QColor(img.pixel(beginPoint, y)));
		for (int x = 1; x < img.width(); x++) {
			nextPause = getPauseFromColor(QColor(img.pixel(x, y)));
			if (currentPause != nextPause) {
				endPoint = x;

				addCommand(TCommandType::lineTo, x, currentPause);

				currentPause = nextPause;
				beginPoint = x;
			}
		}

		endPoint = img.width() - 1;
		addCommand(TCommandType::lineTo, endPoint, currentPause);
		addCommand(TCommandType::newRow);
	}

	commandList->pop_back();
	addCommand(TCommandType::moveToZero);
	addCommand(TCommandType::endPrint);

	generateImage();
	emit commandListReady();
}

void TCommands::doMakeLinesFromBin(QString filename) {
	QFile file(filename);
	if (!file.open(QFile::ReadOnly)) {
		//qDebug() << "can't open file " << filename;
	}
	QByteArray binCommands = file.readAll();

	if (!commandList->isEmpty()) commandList->clear();
	for (int i = 0; i < binCommands.count(); i += 5) {
		TCommand command;
		command.command = binCommands.at(i);
		command.parameter1 = ((uint8_t)binCommands.at(i + 1) << 8) | (uint8_t)binCommands.at(i + 2);
		command.parameter2 = ((uint8_t)binCommands.at(i + 3) << 8) | (uint8_t)binCommands.at(i + 4);

		commandList->push_back(command);
	}

	generateImage();
	emit commandListReady();
}

void TCommands::closeFile() {
	commandList->clear();
}

QPixmap TCommands::getImage() {
	return *image;
}

QByteArray TCommands::getByteArray() {
	QByteArray binCommands;
	char c[5];

	for (int i = 0; i < commandList->count(); i++) {
		binCommands.append(getByteArrayFromCommand(commandList->at(i)));
	}

	return binCommands;
}

uint64_t TCommands::getPrintingTime() {
	return printingTime;
}
