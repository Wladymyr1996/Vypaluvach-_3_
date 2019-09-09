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

#ifndef CNC_H
#define CNC_H

#include <QObject>
#include <QString>
#include <QList>
#include <QSerialPort>
#include <QTimer>
#include <QSerialPortInfo>
#include <QThread>

#include <data.h>
#include <commands.h>

/* Клас роботи з ЧПК верстатом через UART
 * Даний клас погано тестувався. Тому може бути купа баґів
 *
 * Про роботу:
 * Є буфер. Звідти летять дані по UART на Arduino.  Коли Arduino опрацьову,
 * вона надсилає "ехо", після чого, якщо є ще дані в буфері, відправляємо
 * наступний байт.
 *
 * Тобто, що предача даних, як файла на фелш-накопичувач, що друк через
 * UART, це лише відправка зформованого буферу даних (QByteArray)
 */

class TCnc : public QObject
{
	Q_OBJECT
	public:
		explicit TCnc(QObject *parent = nullptr); //Очевидно, що конструктор
		QList<QString> getPortList();
		QString getPortName();
		bool isConnected();
		bool isBusy();

	private:
		void write(QByteArray data);
		void write();
		enum TStatus {None, Echo, Printing, Sending};
		TStatus status;
		QSerialPort *serialPort;
		QString portName;
		QTimer echoTimer;
		int echoCounter;
		QByteArray transferData;
		int indexDataTransfering;

	public slots:
		void beginPrint(QByteArray image);
		void beginTransfer(QByteArray data);
		void sendCommand(TCommands::TCommand command);
		void setPort(QString name);
		void open();
		void close();

	private slots:
		void readFromPort();
		void doEchoTimer();

	signals:
		void connected(bool success);
		void worked(qreal procent);
		void setProgressMax(int);
		void setProgressValue(int);
};

#endif // CNC_H
