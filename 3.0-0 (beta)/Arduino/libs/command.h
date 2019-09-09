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

#ifndef COMMANDLIB
#define COMMANDLIB

enum TCommandType {
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
	errorTransfer = 0xFF
};

struct TCommand {
	uint8_t type;
	int16_t parameter1;
	int16_t parameter2;
};

class TCommandList {
	public:
		void push(TCommand command);
		TCommand pop();
		uint8_t getCount();
		void clear();
		
	private:
		TCommand com[5];
		int8_t index = 0;
		int8_t count = 0;
};

uint8_t TCommandList::getCount() {
	return count;
}

void TCommandList::push(TCommand command) {
	com[index] = command;
	count++;
	if (++index == 5) index = 0;
}

TCommand TCommandList::pop() {
	int8_t retIndex = index - count;
	if (retIndex < 0) retIndex += 5;
	count--;
	return com[retIndex];
}

void TCommandList::clear() {
	count = 0;
}

#endif

