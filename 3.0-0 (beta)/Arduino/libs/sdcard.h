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

#ifndef TSDCARD
#define TSDCARD

#include <SPI.h>
#include <SdFat.h>
#include "command.h"

SdFat SD;

class TSDCard {
	public:
		TSDCard();
		void init();
		void mount();
		void umount();
		bool isMounted();
		bool isOpened();
		bool openFile(int16_t index);
		bool saveFile(String filename);
		void fileWrite(uint8_t *c_str, uint8_t num);
		void closeFile();
		TCommand TSDCard::getNextCommand();
		bool commandAvailable();
		unsigned long getCommandCount();
		
		
		int16_t getItemCount();
		String getItem(int16_t index);
		
	private:
		File root, file;
		uint8_t itemCount = 0;
		bool mounted = false;
		bool opened = false;
		bool forWrite = false;
		void updateRoot();
};

TSDCard::TSDCard() {
	
}

void TSDCard::init() {
	mount();
}

void TSDCard::updateRoot() {
	int16_t count = 0;
	
	root.rewindDirectory();
	while (1) {
		File entry = root.openNextFile();
		if (!entry) break;
		if (!entry.isDirectory()) {
			count++;
		}
		entry.close();
	}
	
	itemCount = count;
}

void TSDCard::mount() {
	if (SD.begin(6)) {
		mounted = true;
		root = SD.open("/");
		
		updateRoot();
	}
}

void TSDCard::umount() {
	root.close();
	mounted = false;
}

bool TSDCard::isMounted() {
	return mounted;
}
	
int16_t TSDCard::getItemCount() {
	return itemCount;
}

String TSDCard::getItem(int16_t index) {
	index += 1;
	int16_t count = 0;
	
	root.rewindDirectory();
	while (1) {
		File entry = root.openNextFile();
		if (!entry) return String("Error");
		if (!entry.isDirectory()) {
			char name[16];
			entry.getName(name, 16);
			count++;
			if (count == index) {
				entry.close();
				return String(name);
			}
		}
		entry.close();
	}
}

bool TSDCard::isOpened() {
	return opened;
}

bool TSDCard::openFile(int16_t index) {
	String filename = getItem(index);
	
	file = SD.open("/" + filename, FILE_READ);
	if (file) {
		opened = true;
		forWrite = false;
		return true;
	} else {
		opened = false;
		return false;
	}
}

bool TSDCard::saveFile(String fileName) {
	file = SD.open("/" + fileName, FILE_WRITE);
	if (file) {
		opened = true;
		forWrite = true;
		return true;
	} else {
		opened = false;
		return false;
	}
}

void TSDCard::fileWrite(uint8_t *c_str, uint8_t num) {
	if (opened && forWrite) {
		file.write(c_str, num);
	}
}

void TSDCard::closeFile() {
	if (file.isOpen()) file.close();
}

TCommand TSDCard::getNextCommand() {
	TCommand command;
	if (file.available() && (!forWrite)) {
		char data[5];
		
		for (uint8_t i = 0; i < 5; i++) 
			data[i] = file.read();
			
		command.type = data[0];
		command.parameter1 = ((uint8_t)data[1] << 8) | (uint8_t)data[2];
		command.parameter2 = ((uint8_t)data[3] << 8) | (uint8_t)data[4];
	}
	return command;
}

bool TSDCard::commandAvailable() {
	if (!mounted) return false;
	if (!opened) return false;
	if (file.available())
		return true;
	else 
		return false;
}

unsigned long TSDCard::getCommandCount() {
	return file.size() / 5;
}

#endif

