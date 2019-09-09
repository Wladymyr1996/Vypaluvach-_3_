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

#include "imageviewer.h"

TImageViewer::TImageViewer(QWidget *parent) : QWidget(parent) {
	updateLabels();
}

bool TImageViewer::imageIsOpened() {
	return isOpened;
}

void TImageViewer::paintEvent(QPaintEvent *event) {
	QPainter painter(this); //Створюємо полотно
	QFontMetrics fm(painter.font());

	//Фон
	painter.setBrush(QBrush(Qt::gray));
	painter.setPen(QPen(Qt::black, 1, Qt::SolidLine));
	painter.drawRect(QRectF(0, 0, width()-1, height()-1));

	if (!isOpened) {
		//задля красивих діагональних ліній
		painter.setRenderHint(QPainter::Antialiasing);

		//Діагональні лінії
		painter.drawLine(0,0,width(),height());
		painter.drawLine(width(),0,0,height());

		//Текст "Зображення відсутнє"
		QSize textSize(fm.width(labelNoImage), fm.height());
		QRect textFrame;
		textFrame.setX((width() - textSize.width()) / 2);
		textFrame.setY((height() - textSize.height()) / 2);
		textFrame.setSize(textSize);
		QRect frame(textFrame.x() - 2, textFrame.y() - 2, textFrame.width() + 4, textFrame.height() + 4);
		painter.setBrush(QBrush(QColor(200, 100, 100)));
		painter.drawRoundRect(frame, 15, 15);
		painter.setBrush(Qt::white);
		painter.setPen(Qt::white);
		textFrame.setWidth(width()); //Костилі, але без нього не працює (Дивні дива)
		painter.drawText(textFrame, labelNoImage);
	} else {
		painter.drawPixmap(imgFrame, *picture);
		painter.drawText(QRect(2, height() - fm.height() - 4, fm.width(labelPrintingTime), fm.height()), labelPrintingTime);
	}
}

void TImageViewer::resizeEvent(QResizeEvent *event) {
	if (isOpened) {
		updateImagePos();
	}
}

void TImageViewer::updateLabels() {
	labelNoImage = tr("NO IMAGE");
	{
		int day = printingTime / 86400000;
		int hour = (printingTime / 3600000) % 24;
		int minute = (printingTime / 60000) % 60;
		int second = (printingTime / 1000) % 60;

		labelPrintingTime = tr("Printing time: ");
		if (day > 0)
			labelPrintingTime += QString::number(day) + tr("d ", "time");
		if (hour > 0)
			labelPrintingTime += QString::number(hour) + tr("h ", "time");
		labelPrintingTime += QString::number(minute) + tr("m ", "time");
		labelPrintingTime += QString::number(second) + tr("s ", "time");
	}
	repaint();
}

void TImageViewer::setImage(QPixmap image) {
	if (!isOpened) {
		picture = new QPixmap();
		isOpened = true;
	}
	*picture = image;

	updateImagePos();
	repaint();
}

void TImageViewer::closeImage() {
	isOpened = false;
	delete picture;
	repaint();
}

void TImageViewer::updateImagePos() {
	int newWidth = width(), newHeight = height();
	if ((picture->width())*newHeight > picture->height()*newWidth) {
		scale = (qreal)(newWidth-2) / (qreal)(picture->width());
	} else scale = (qreal)(newHeight-2) / (qreal)(picture->height());

	QSize newSize;
	newSize.setWidth(qRound((qreal)picture->width() * scale));
	newSize.setHeight(qRound((qreal)picture->height() * scale));

	imgFrame.setX((width() - newSize.width() - 1) / 2 + 1);
	imgFrame.setY((height() - newSize.height() - 1) / 2 + 1);
	imgFrame.setSize(newSize);

	repaint();
}

void TImageViewer::setPrintingTime(uint64_t time) {
	printingTime = time;
	updateLabels();
	repaint();
}
