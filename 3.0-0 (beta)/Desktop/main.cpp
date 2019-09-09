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

#include <QApplication>
#include <QTranslator>

#include "mainwindow.h"
#include "data.h"

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	a.setFont(QFont());
	QTranslator translator;

	MainWindow w(&translator);
	a.installTranslator(&translator);
	w.doSetLang(glData->getLanguage());

	w.show();

	return a.exec();
}
