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

#ifndef PCLIB
#define PCLIB

#include "settings.h"
#include "command.h"

//Робота з UART

class TCommunication {
	public:
		void init();
		void work();
		bool commandAvailable();
		void commandAvailableWait();
		TCommand getCommand();
		String readName();
		void getNextCommand();
		void clearCommand();
		
	private:
		bool needRead = false;
		bool isReaded = false;
		TCommand commandTmp;
};

void TCommunication::init() {
	Serial.begin(BAUDRATE);
	needRead = true;
}

void TCommunication::work() {
	if ((Serial.available() >= 5) && (!isReaded) && (needRead)) {
		char data[5];
		
		for (uint8_t i = 0; i < 5; i++) 
			data[i] = Serial.read();
		
		commandTmp.type = data[0];
		commandTmp.parameter1 = ((uint8_t)data[1] << 8) | (uint8_t)data[2];
		commandTmp.parameter2 = ((uint8_t)data[3] << 8) | (uint8_t)data[4];
		
		if (commandTmp.type == TCommandType::echo) {
			Serial.write(TCommandType::echo);
			Serial.write(TCommandType::echo);
		} else {
			isReaded = true;
		}
	}
}

bool TCommunication::commandAvailable() {
	bool ret;
	asm volatile (\
		"lds %[RET], %[IS_READED]	\n" \
	:[RET]"=r" (ret) :[IS_READED]"s" (&isReaded));
	return ret;
}

void TCommunication::commandAvailableWait() {
	asm volatile (\
		"push r23					\n" \
		"TCommunicationWaitLoop: 	\n" \
		"	lds r23, %[IS_READED]	\n" \
		"	cpi r23, %[TRUE]		\n" \
		"	brne TStepperWaitLoop	\n" \
		"pop r23					\n" \
	::[IS_READED]"s" (&isReaded), [TRUE]"M" (true));
}

TCommand TCommunication::getCommand() {
	if (isReaded) {
		isReaded = false;
		getNextCommand();
		return commandTmp;
	}
	return (TCommand){0xff, 0, 0};
}

String TCommunication::readName() {
	needRead = false;
	
	char c;
	String name;
	do {
		uint16_t timer = 0;
		while (Serial.available() == 0) {
			timer++;
			delay(1);
			if (timer == 20000) return "";
		};
		c = Serial.read();
		Serial.write(TCommandType::echo);
	} while(c != TCommandType::beginFileName);
	do {
		uint16_t timer = 0;
		while (Serial.available() == 0) {
			timer++;
			delay(1);
			if (timer == 20000) return "";
		};
		c = Serial.read();
		if (((c >= 0x30) && (c <= 0x39)) || ((c >= 0x41) && (c <= 0x5A)) || ((c >= 0x61) && (c <= 0x7A)) || (c == 0x20) || (c == 0x5F) || (c == 0x2E)) {
			name += c;
		}
		if (c != TCommandType::endFileName) Serial.write(TCommandType::echo);
	} while(c != TCommandType::endFileName);
	
	needRead = true;
	return name;
}

void TCommunication::getNextCommand() {
	Serial.write(TCommandType::echo);
	Serial.write(TCommandType::echo);
	Serial.write(TCommandType::echo);
	Serial.write(TCommandType::echo);
	Serial.write(TCommandType::echo);
}

void TCommunication::clearCommand() {
	while (Serial.available()) {
		getCommand();
	}
}

#endif
