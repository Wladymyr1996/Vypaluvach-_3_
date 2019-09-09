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

#ifndef TDISPLAY
#define DISPLAY

#include "settings.h"

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

class TDisplay {
	public:
		TDisplay::TDisplay();
		void DrawWelcome();
		void DrawMenu(String title, String item, uint8_t index, uint8_t count); //, bool animation = false, bool left =  true);
		void DrawPrintingScreen(uint8_t procent); 
		void DrawMessage(String title, String text);
		void DrawManualMoving(String title, int16_t pos);
		void DrawTransferingDisplay(String title, String text);
		
	private:
		LiquidCrystal_I2C lcd = {0x27, 16, 2};
};

TDisplay::TDisplay() {
		
}

void TDisplay::DrawWelcome() {
	lcd.clear();
	lcd.init();
	lcd.backlight();
	lcd.home();
	lcd.print("Vypaluvach  v" "3.0");
	lcd.setCursor(0, 1);
	lcd.print(" shaitech.pp.ua");
	
	{
		char sym[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0x00};
		lcd.createChar(0, sym);
	}
	
	delay(1000);
}

void TDisplay::DrawMenu(String title, String item, uint8_t index, uint8_t count) { //, bool animation, bool left) {
	//Наступні рядки, проба анімацій в меню :D
	/*
	static String oldItem = "";
	static bool anim = false;
	if (animation) {
		anim = true;
		String view;
		if (left) {
			view = oldItem;
			while (view.length() < 20) view += " ";
			view += item;
			for (uint8_t i = 0; i < 20; i++) {
				DrawMenu(title, view.substring(i, i+14), index, count);
				delay(5);
			}
		} else {
			view = item;
			while (view.length() < 20) view += " ";
			view += oldItem;
			for (uint8_t i = 20; i > 0; i--) {
				DrawMenu(title, view.substring(i, i+14), index, count);
				delay(5);
			}
		}
		anim = false;
	}
	if (!anim) oldItem = item;
	*/
	
	String items = String(index, DEC) + String("/") + String(count, DEC);
	
	if (items.length() + title.length() > 15) {
		title.remove(14 - items.length());
		title += (char)0x00;
	}
	
	while (title.length() + items.length() < 16) title += " ";
	title += items;
	
	while (item.length() < 14) item += " ";
	
	if (item.length() > 14) {
		item.remove(13);
		item += (char)0x00;
	}
	
	//lcd.clear();
	lcd.home();
	lcd.print(title);
	lcd.setCursor(0, 1);
	lcd.print((char)0x7F);
	lcd.print(item);
	lcd.setCursor(15, 1);
	lcd.print((char)0x7E);
}

void TDisplay::DrawPrintingScreen(uint8_t procent) {
	String str = strPrinting;
	String p(procent, DEC);
	p += "%";
	
	while (str.length() + p.length() < 16) str += " ";
	
	lcd.home();
	lcd.print(strPrintingTitle);
	lcd.setCursor(0, 1);
	lcd.print(str);
	lcd.print(p);
}

void TDisplay::DrawMessage(String title, String message) {
	while (title.length() < 16) title += " ";
	while (message.length() < 16) message += " ";
	lcd.home();
	lcd.print(title);
	lcd.setCursor(0, 1);
	lcd.print(message);
}

void TDisplay::DrawManualMoving(String title, int16_t pos) {
	while (title.length() < 16) title += " ";
	String str = strPos;
	String spos = String(pos, DEC);
	while (str.length() + spos.length() < 16) str += " ";
	str += spos;
	
	lcd.home();
	lcd.print(title);
	lcd.setCursor(0, 1);
	lcd.print(str);
}

void TDisplay::DrawTransferingDisplay(String title, String text) {
	DrawMessage(title, text);
}

#endif

