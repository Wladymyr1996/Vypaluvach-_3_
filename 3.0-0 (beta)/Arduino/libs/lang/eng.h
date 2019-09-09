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

#ifndef LANGLIB
#define LANGLIB

//файл мови. Якщо бажаєте додати іншу мову, копіюйте цей файл і перекладайте своєю мовою

//MainMenu
#define strMainMenu "Main Menu" 
#define strPrintFile "Print file"
#define strMount "Mount SD card" 
#define strUnmount "Umount SD card"
#define strUploadFromPC "Upload from PC"
#define strPrintFromPC "Print from PC"
#define strManualMove "Manual move" //Ручне керування
 
//FileSelector
#define strSelectFile "Select file" //Обрати файл

//Printing screen
#define strPrinting "Printing"
#define strPrintingTitle "Vypaluvach | ShT"

//Messages
#define strMessage "Message"
#define strError "Error"	
#define strPrintingCanceled "Print is cancel"
#define strPrintingSuccess "Printing success"

//MoveMenu
#define strMoveX "Move X axis"
#define strMoveY "Move Y axis"
#define strPos "Pos:"

//Transfer
#define strTransferFile "Transfer file"
#define strTransfering "Transfering" + (char)0x00
#define strPrintingFromUSB "Printing from PC"
#define strPrintingDots "Printing" + (char)0x00

#endif


