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

#ifndef STEPPERLIB
#define STEPPERLIB

//Крокові дввигуни рахують інтервали по таймеру у перериванні
//Тому поки крокові двигуни крутяться, процесор може виконувати
//якісь свої процесорські справи

struct TPos {
	int16_t x;
	int16_t y;
};

class TStepper {
	public:
		TStepper();
		void moveX(int16_t toX, uint16_t interval);
		void moveY(int16_t toY, uint16_t interval);
		void work();
		void goHome();
		TPos getPos();
		void setPos(TPos newPos);
		void setPos(int16_t x, int16_t y);
		bool isBusy();
		volatile void wait();
		
	private:
		enum TStatus {none, movingX, movingY};
		void doStepX(int direct);
		void doStepY(int direct);
	
		TStatus status;
		TPos pos;
		int16_t moveTo;
		uint16_t interval;
};

TStepper::TStepper() {
	PORTB &= 0xF8;
	PORTD &= 0x73;
	DDRB |= 0x07;
	DDRD |= 0x8C;
	
	status = TStatus::none;
}

void TStepper::moveX(int16_t toX, uint16_t interval) {
	moveTo = toX;
	this->interval = interval;
	status = TStatus::movingX;
}

void TStepper::moveY(int16_t toY, uint16_t interval) {
	moveTo = toY;
	this->interval = interval;
	status = TStatus::movingY;
}

void TStepper::work() {
	static uint16_t timer = 0;
	switch (status) {
		case TStatus::movingX: {
			timer++;
			if (timer == interval) {
				if (pos.x > moveTo) 
					doStepX(-1);
				else
					if (pos.x == moveTo) 
						status = TStatus::none;
					else
						doStepX(1);
					
				timer = 0;
			}
		} break;
		case TStatus::movingY: {
			timer++;
			if (timer == interval) {
				if (pos.y > moveTo) 
					doStepY(-1);
				else
					if (pos.y == moveTo) 
						status = TStatus::none;
					else
						doStepY(1);
					
				timer = 0;
			}
		} break;
	}
}

void TStepper::goHome() {
	wait();
	moveX(0, 2);
	wait();
	moveY(0, minSpeedY);
	wait();
}

TPos TStepper::getPos() {
	return pos;
}

bool TStepper::isBusy() {
	if (status == TStatus::none) 
		return false;
	else 
		return true;
}

void TStepper::doStepX(int direct) {
	if (direct == 1) {
		PORTD |= 0x04;
	} else {
		PORTD &= 0xFB;
	}
	pos.x += direct;
	
	PORTD |= 0x08;
	asm("nop");
	PORTD &= 0xF7;
}

void TStepper::doStepY(int direct) {
	const uint8_t steps[8] = {0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08, 0x09};
	static uint8_t index = 0;
	index += direct;
	pos.y += direct;
	
	if (index == 8) 
		index = 0;
	else 
		if (index == 255) 
			index = 7;
	
	PORTB &= 0xF8;
	PORTD &= 0x7F;
	uint8_t out = steps[index];
	PORTB |= out >> 1;
	if (out & 0x01) 
		PORTD |= 0x80;
}

void TStepper::setPos(TPos newPos) {
	pos = newPos;
}

void TStepper::setPos(int16_t x, int16_t y) {
	pos.x = x;
	pos.y = y;
}

volatile void TStepper::wait() {
	asm volatile (\
		"push r23					\n" \
		"TStepperWaitLoop: 			\n" \
		"	lds r23, %[STATUS]		\n" \
		"	cpi r23, %[NONESTATUS]	\n" \
		"	brne TStepperWaitLoop	\n" \
		"pop r23					\n" \
	:: [STATUS]"s" (&status), [NONESTATUS]"M" (TStatus::none));
}

#endif
