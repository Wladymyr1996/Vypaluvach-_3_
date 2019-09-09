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

#ifndef COMMANDS_H
#define COMMANDS_H

#include <QObject>
#include <QPixmap>
#include <QByteArray>
#include <QTime>
#include <QImage>
#include <QPixmap>
#include <QPainter>

#include "data.h"

/*
 * Клас формування, зберігання та обробки масиву команд
 */

class TCommands : public QObject {
	Q_OBJECT

	public:
		enum TCommandType { //Набір команд
			echo = 0xAA,
			getVersion = 0x01,
			lineTo = 0x02,
			newRow = 0x03,
			moveToZero = 0x04,
			endPrint = 0x05,
			setColorCount = 0x06,
			setMinPauseX = 0x07,
			setSkippiedRows = 0x08,
			setSkippiedYSteps = 0x09,
			setImageSize = 0x0A,
			beginPrint = 0x0B,
			pause = 0x0C,
			setAsZero = 0x0D,
			setVersion = 0x0E,
			beginFileName = 0x0F,
			endFileName = 0x10,
			endTransfering = 0x11,
			errorTransfer = 0xFF6
		};
		struct TCommand { //структура команди (завжди 5 байт)
			uint8_t command;
			uint16_t parameter1;
			uint16_t parameter2;
		};
		explicit TCommands(QObject *parent = nullptr);
		QPixmap getImage(); //Повертає зображення поточне
		QByteArray getByteArray(); //Повертає мачив команд
		uint64_t getPrintingTime(); //Повертає розрахунковвий час друку
		static TCommand getCommand(uint8_t command, uint16_t parameter1 = 0, uint16_t paramerter2 = 0); //Формування команди
		static TCommand getCommand(QByteArray data); //Формування команди із масиву
		static QByteArray getByteArrayFromCommand(TCommand command); //Отримати масив із команди


	private:
		void addCommand(uint8_t command, uint16_t parameter1 = 0, uint16_t paramerter2 = 0); //Додати команду
		void generateImage(); //Згенерувати зображення із набору команд
		QPixmap *image; //Зображення

		uint16_t getPauseFromColor(QColor color); //Повертає інтервал між кроками базуючись на кольорі
		uint64_t printingTime; //Час друку
		QColor getColorFromPause(uint16_t pause, int colorCount); //Повертає колір базуючись на інтервалах
		QList<TCommand> *commandList; //Масив команд

	signals:
		void commandListReady();
		void imageReady(QPixmap*);

	public slots:
		void doMakeLinesFromImage(QString filename);
		void doMakeLinesFromBin(QString filename);
		void closeFile();
};

#endif // COMMANDS_H
