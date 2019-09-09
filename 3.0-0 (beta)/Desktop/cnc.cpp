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

#include "cnc.h"

TCnc::TCnc(QObject *parent) : QObject(parent) {
	serialPort = new QSerialPort(this);
	status = TStatus::None;

	connect(&echoTimer, SIGNAL(timeout()), SLOT(doEchoTimer()));
	connect(serialPort, SIGNAL(readyRead()), SLOT(readFromPort()));
}

QList<QString> TCnc::getPortList() {
	QList<QString> list;
	QList<QSerialPortInfo> listPortInfo = QSerialPortInfo::availablePorts();

	for (int i = 0; i < listPortInfo.count(); i++) {
		list.push_back(listPortInfo.at(i).portName());
	}

	return list;
}

QString TCnc::getPortName() {
	return portName;
}

bool TCnc::isConnected() {
	return serialPort->isOpen();
}

bool TCnc::isBusy() {
	if (status != TStatus::None) return true;
	else return false;
}

void TCnc::open() {
	serialPort->setPort(QSerialPortInfo(portName));
	serialPort->setBaudRate(glData->getBoudRate());
	if (!serialPort->open(QSerialPort::ReadWrite)) {
		emit connected(false);
		return;
	}
	status = TStatus::Echo;
	QThread::sleep(2);
	write(TCommands::getByteArrayFromCommand(TCommands::getCommand(TCommands::TCommandType::echo)));
}

void TCnc::close() {
	serialPort->close();
	emit connected(false);
}

void TCnc::write(QByteArray data) {
	serialPort->write(data);
}

void TCnc::write() {
	if (indexDataTransfering < transferData.count())
		write(QByteArray(1, transferData[indexDataTransfering++]));
}

void TCnc::beginPrint(QByteArray image) {
	if (isConnected()) {
		transferData = image;
		indexDataTransfering = 0;
		status = TStatus::Sending;
		emit setProgressMax(transferData.count());
		emit setProgressValue(0);

		for (int i = 0; i < 5; i++)
			write();
	}
}

void TCnc::beginTransfer(QByteArray data) {
	if (isConnected()) {
		transferData = data;
		indexDataTransfering = 0;
		status = TStatus::Sending;
		emit setProgressMax(transferData.count());
		emit setProgressValue(0);

		write();
	}
}

void TCnc::sendCommand(TCommands::TCommand command) {
	QByteArray data(TCommands::getByteArrayFromCommand(command));
	if (isConnected()) {
		write(data);
	}
}

void TCnc::setPort(QString name) {
	portName = name;
}

void TCnc::readFromPort() {
	QByteArray data = serialPort->readAll();
	//qDebug() << "CNC: " << data.toHex();

	switch (status) {
		case TStatus::Echo: {
			if (data.count() >= 2) {
				for (int i = 0; i < data.count() - 1; i++) {
					if ((data.at(i) == (char)TCommands::TCommandType::echo) && (data.at(i+1) == (char)TCommands::TCommandType::echo)) {
						echoTimer.stop();
						status = TStatus::None;
						emit connected(true);
					}
				}
			}
		} break;

		case TStatus::Sending: {
			if (data.at(0) == TCommands::TCommandType::errorTransfer) {
				status = TStatus::None;
				break;
			}
			for (int i = 0; i < data.count(); i++) {
				if (data.at(i) == (char)TCommands::TCommandType::echo)
					write();
				emit setProgressValue(indexDataTransfering);
			}

		} break;
	}
}

void TCnc::doEchoTimer() {
	echoCounter++;
	if (echoCounter == 50) {
		echoTimer.stop();
		serialPort->close();
		status = TStatus::None;
		emit connected(false);
	}
}
