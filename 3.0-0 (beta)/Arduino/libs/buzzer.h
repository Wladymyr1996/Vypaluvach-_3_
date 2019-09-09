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

#ifndef TBUZZER
#define TBUZZER

class TBuzzer {
	public:
		TBuzzer();
		void beep(int8_t count = 1);
		void work();
	private:
		const uint8_t pin = PD5;
		uint8_t timer = 0;
		int8_t counter = 0;
};

TBuzzer::TBuzzer() {
	PORTD &= ~(1 << pin);
	DDRD |= 1 << pin;
}

void TBuzzer::beep(int8_t count) {
	if (count < -1) count = 0;
	counter = count;
	PORTD |= 1 << pin;
	timer = 50;
}

void TBuzzer::work() {
	if (counter == -1) {
		timer--;
		if (timer == 25) PORTD &= ~(1 << pin);
	} else if (counter > 0) {
		timer--;
		if (timer == 25) PORTD &= ~(1 << pin);
		if (timer == 0) counter--;
	};
}

extern TBuzzer buzzer;

#endif

