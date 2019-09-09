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

#include <TimerOne.h> //Клас таймера
#include "libs/display.h" //Клас роботи з екраном
#include "libs/buttons.h" //Клас роботи з кнопками
#include "libs/buzzer.h" //Клас роботи з пищалкою
#include "libs/sdcard.h" //Клас роботи з SD картою пам'яті
#include "libs/stepper.h" //Клас роботи з кроковими двигунами
#include "libs/supply.h" //Клас керування живленням
#include "libs/command.h" //Клас обробки команд
#include "libs/settings.h" //Клас налаштувань
#include "libs/pc.h" //Клас роботи з комп'ютером по UART (beta)

TDisplay display;
TBuzzer buzzer;
TButtons keyboard;
TSDCard card;
TStepper stepper;
TCommandList commandList;
TCommunication communication;

bool repaint = true; //Чи потрібно перемальовавти екран
bool serialWork = false; //Чи йде робота з Serial
int8_t ystepscount = 1; //Кількість кроків на рядок

void work() {
	//Псевдобагатозадачність
	keyboard.work();
	buzzer.work();
	stepper.work();
	communication.work();
	
	if (!serialWork)
		if (communication.commandAvailable()) {
			communication.clearCommand();
			Serial.write(TCommandType::errorTransfer);
		}	
}

void drawMessage(String title, String message, uint16_t timer = 5000) {
	//Вивести повідомлення
	display.DrawMessage(title, message);
	uint16_t counter = 0;
	while (keyboard.getLastClicked() == TButtons::Button::None) {
		if (timer != 0) {
			counter += 100;
			if (counter >= timer) break;
		}
		delay(100);
	}
} 

void setup() {
	//Ініціалізація
	communication.init();
	
	display.DrawWelcome();
	
	Timer1.initialize(1000);
	Timer1.attachInterrupt(work);
	
	card.init();
	Supply::init();
}

void saveFile() {
	//Зберігання файлу по UART (як виявилось, це проходить довго) :D
	display.DrawTransferingDisplay(strTransferFile, strTransfering);
	
	serialWork = true;
	
	String fileName = communication.readName();
	if (fileName == "")  {
		serialWork = false;
		return;
	}
	
	if (!card.saveFile(fileName)) {
		serialWork = false;
		return;
	}

	display.DrawTransferingDisplay(fileName, strTransfering);

	communication.getNextCommand();
	TCommand com;
	do {
		communication.commandAvailableWait();
		com = communication.getCommand();
		
		uint8_t data[5];
		data[0] = com.type;
		data[1] = com.parameter1 >> 8;
		data[2] = com.parameter1;
		data[3] = com.parameter2 >> 8;
		data[4] = com.parameter2;
		
		card.fileWrite(data, 5);
	} while (com.type != TCommandType::endPrint);
	card.closeFile();
	
	serialWork = false;
}

void doNextCommand() {
	//Виконати наступну команду з буферу команд
	TCommand command = commandList.pop();
	
	switch (command.type) {
		case TCommandType::setSkippiedYSteps: {
			ystepscount = command.parameter1;
		} break;
		case TCommandType::setAsZero: {
			stepper.setPos(0, 0);
		}
		case TCommandType::beginPrint: {
			Supply::switchOn();
		} break;
		case TCommandType::endPrint: {
			stepper.goHome();
			
			Supply::switchOff();
			buzzer.beep(5);
		} break;
		case TCommandType::pause: {
			delay(command.parameter1 * 1000);
		} break;
		case TCommandType::lineTo: {
			stepper.wait();
			stepper.moveX(command.parameter1, command.parameter2);
		} break;
		case TCommandType::newRow: {
			stepper.wait();
			stepper.moveX(-100, 2);
			repaint = true;
			stepper.wait();
			stepper.moveX(0, 5);
			stepper.wait();
			stepper.moveY(stepper.getPos().y - ystepscount, minSpeedY);
			stepper.wait();
		} break;
	}
}

void printUART() {
	//Друк по UART (beta)
	display.DrawTransferingDisplay(strPrintingFromUSB, strPrintingDots);
	TCommand com;
	
	
	serialWork = true;
	for (uint8_t i = 0; i < 5; i++) {
		communication.commandAvailableWait();
		com = communication.getCommand();
		commandList.push(com);
	}	
	
	do {
		communication.commandAvailableWait();
		com = communication.getCommand();
		commandList.push(com);
		
		if (keyboard.getLastClicked() == TButtons::Button::Back)
			commandList.push((TCommand){TCommandType::endPrint, 0, 0});
		
		doNextCommand();		
	} while (com.type != TCommandType::endPrint);
	serialWork = false;
}

void printFile(int16_t index) {
	//Друк з флешки
	if (card.openFile(index)) {
		//Дізнаємось відомості про файл, аби розрахувати відсотки
		unsigned long commandCount = card.getCommandCount();
		unsigned long currentCommand = 5;
		
		//Заповнюємо масив першою партією команд
		commandList.clear();
		for (uint8_t i = 0; i < 5; i++) {
			commandList.push(card.getNextCommand());
		}
			
		display.DrawPrintingScreen(0);
		
		//Починаємо друк
		while (commandList.getCount() != 0) {
			//Запускаємо виконання наступної команди
			doNextCommand();
				
			//Доки команда виконується, замаємось своїми справами, а саме дістаємо наступну команду
			currentCommand++;
			if (card.commandAvailable()) {
				commandList.push(card.getNextCommand());
			}
			
			
			//Якщо користува відмінив друк
			if (keyboard.getLastClicked() == TButtons::Button::Back) {
				commandList.clear();
				card.closeFile();
				display.DrawMessage(strMessage, strPrintingCanceled);
				
				stepper.goHome();
				Supply::switchOff();
				
				drawMessage(strMessage, strPrintingCanceled);
				return;
			}
			
			//Перемалювання
			if (repaint) {
				unsigned long procent = (100 * currentCommand) / commandCount;
				display.DrawPrintingScreen(procent);
			}
		}
		
		drawMessage(strMessage, strPrintingSuccess, 0);
	}
}

void selectFile() {
	//Меню файлів
	repaint = true;
	
	if (!card.isMounted()) return;
	
	if (card.getItemCount() == 0) {
		//Якщо відповідних файлів нема
		display.DrawMenu(strSelectFile, "", 0, 0);
		do {
			delay(100);
		} while (keyboard.getLastClicked() != TButtons::Button::Back);
		return;
	} else {
		int16_t index = 0; //Потичний пункт
		int16_t count = card.getItemCount(); // Всього пунктів
		
		while (1) {
			TButtons::Button key = keyboard.getLastClicked();
			if (key != TButtons::Button::None) {
				switch (key) {
					case TButtons::Button::Up: 
						index--; break;
					case TButtons::Button::Down:
						index++; break;
					case TButtons::Button::Back:
						return;
					case TButtons::Button::Ok: 
						printFile(index); break;
				}
				
				if (index < 0) index = 0;
				if (index >= count) index = count - 1;
				
				repaint = true;
			}
			
			if (repaint) {
				display.DrawMenu(strSelectFile, card.getItem(index), index + 1, count);	
				repaint = false;
			}
			delay(100);
		}
	}
}

void movingXMenu() {
	//Ручний рух по вісі X
	repaint = true;
	bool exit = false;
	
	while (1) {
		TButtons::Button key = keyboard.getPressed();
		if (key != TButtons::Button::None) {	
			switch (key) {
				case TButtons::Button::Up: {
					stepper.moveX(stepper.getPos().x + 10, 2);
					stepper.wait();
				} break;
				case TButtons::Button::Down: {
					stepper.moveX(stepper.getPos().x - 10, 2);
					stepper.wait();
				} break;
				case TButtons::Button::Back:
					while (keyboard.getLastClicked() != TButtons::Button::Back) delay(100);
					return;
			}
			
			repaint = true;
		}
		
		if (repaint) {
			display.DrawManualMoving(strMoveX, stepper.getPos().x);
			repaint = false;
		}
	}
}

void movingYMenu() {
	//Ручний рух по вісі Y
	repaint = true;
	bool exit = false;
	
	while (1) {
		TButtons::Button key = keyboard.getPressed();
		if (key != TButtons::Button::None) {	
			switch (key) {
				case TButtons::Button::Up: {
					stepper.moveY(stepper.getPos().y + 10, 3);
					stepper.wait();
				} break;
				case TButtons::Button::Down: {
					stepper.moveY(stepper.getPos().y - 10, 3);
					stepper.wait();
				} break;
				case TButtons::Button::Back:
					while (keyboard.getLastClicked() != TButtons::Button::Back) delay(100);
					return;
			}
			
			repaint = true;
		}
		
		if (repaint) {
			display.DrawManualMoving(strMoveY, stepper.getPos().y);
			repaint = false;
		}
	}
}

void manualMoveMenu() {
	//Меню ручного руху каретки
	repaint = true;
	bool exit = false;
	int8_t index = 0;
	Supply::switchOn();
	
	while (!exit) {
		TButtons::Button key = keyboard.getLastClicked();
		if (key != TButtons::Button::None) {	
			switch (key) {
				case TButtons::Button::Up: 
					index--; break;
				case TButtons::Button::Down:
					index++; break;
				case TButtons::Button::Ok: {
					switch (index) {
						case 0:
							movingXMenu(); break;
						case 1:
							movingYMenu(); break;
					}
				} break;
				case TButtons::Button::Back:
					exit = true; break;
			}
			
			repaint = true;
		}
		
		if (index == -1) index = 0;
		if (index == 2) index = 1;
		
		if (repaint) {
			String item;
			switch (index) {
				case 0: 
					item = strMoveX; break;
				case 1:
					item = strMoveY; break;
			}
			
			display.DrawMenu(strManualMove, item, index + 1, 4);
			repaint = false;
		}
	}
	
	Supply::switchOff();
}

void loop() {
	//Головне меню
	
	static int8_t index = 0;
	
	TButtons::Button key = keyboard.getLastClicked();
	if (key != TButtons::Button::None) {		
		switch (key) {
			case TButtons::Button::Up: 
				index--; break;
			case TButtons::Button::Down:
				index++; break;
			case TButtons::Button::Ok: {
				switch (index) {
					case 0:
						selectFile(); break;
					case 1:
						if (!card.isMounted()) card.mount();
						else card.umount(); break;
					case 2:
						saveFile(); break;
					case 3:
						printUART(); break;
					case 4:
						manualMoveMenu(); break;
				}
			} break;
		}
		
		if (index == -1) index = 0;
		if (index == 5) index = 4;
		
		repaint = true;
	};
	
	if (repaint) {
		String item;
		switch (index) {
			case 0: 
				item = strPrintFile; break;
			case 1:
				if (card.isMounted()) item = strUnmount;
				else item = strMount; break;
			case 2:
				item = strUploadFromPC; break;
			case 3: 
				item = strPrintFromPC; break;
			case 4:
				item = strManualMove; break;
		}
		
		display.DrawMenu(strMainMenu, item, index + 1, 5);
		repaint = false;
	}
	delay(100); 
}
