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

#ifndef TBUTTONS
#define BUTTONS

#include "buzzer.h"

class TButtons {
	public:
		enum Button {Up, Down, Back, Ok, None};
		TButtons();
		Button getPressed();
		Button getLastClicked();
		void work();
		
	private:
		Button lastClicked = Button::None;
};

TButtons::TButtons() {
	DDRC &= 0xF0;
	PORTC &= 0xF0;
}

TButtons::Button TButtons::getPressed() {
	if (PINC & 0x01) return Button::Up;
	if (PINC & 0x02) return Button::Back;
	if (PINC & 0x04) return Button::Ok;
	if (PINC & 0x08) return Button::Down;
	return Button::None;
}

TButtons::Button TButtons::getLastClicked() {
	Button tmp = lastClicked;
	lastClicked = Button::None;
	return tmp;
}

void TButtons::work() {
	static uint8_t pressedTimer = 0;
	static Button oldBtn = Button::None;
	
	Button Btn = getPressed();
	
	if ((pressedTimer > 0) && (pressedTimer < 3)) {
		pressedTimer++;
		if (pressedTimer == 3) {
			if (Btn != Button::None)
				pressedTimer = 1;
		}
		return;
	}
	
	if ((pressedTimer >= 3) && (pressedTimer < 6)) {
		pressedTimer++;
		if (pressedTimer == 6) {
			if (Btn != Button::None) pressedTimer = 50;
			else {
				pressedTimer = 0;
				lastClicked = oldBtn;
				oldBtn = Button::None;
				buzzer.beep();
			}
		}
		return;
	}
	
	if (Btn == Button::None) return;
	
	oldBtn = Btn;
	pressedTimer = 1;
}

#endif


