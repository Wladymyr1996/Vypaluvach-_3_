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

#include "aboutwindow.h"

TAboutWindow::TAboutWindow(QWidget *parent) {
	//parent — то посилання на головвне вікно.
	//Загалом лише для того, аби можна було дізнтися поточні
	//розміри головного вікна

	new QLabel("<img src='://img/logo.png'>", this);
	this->parent = parent;
	textEdit = new QTextEdit(this);
	textEdit->setGeometry(0, 100, 400, 300);
	textEdit->setReadOnly(true);

	setFixedSize(400, 400);
	setWindowFlags(Qt::WindowStaysOnTopHint); //Завжди поверх головного вікна
}

void TAboutWindow::closeEvent(QCloseEvent *) {
	emit opened(false);
}

void TAboutWindow::showEvent(QShowEvent *) {
	updateLabels();

	//Це вікно завжди відкривається по середні голвного вікна
	int x, y;
	x = parent->geometry().x()+(parent->geometry().width()-size().width())/2;
	y = parent->geometry().y()+(parent->geometry().height()-size().height())/2;
	setGeometry(x, y, size().width(), size().height());
	emit opened(true);
}

void TAboutWindow::updateLabels() {
	textEdit->setText(tr("Vypaluvach is a program for a CNC machine tool for burning wood images. The program is free. If there are questions, the author will help you deal with them.") + "\n\n" +
					  tr("Author: Stadnyk Volodymyr") + "\n" +
					  tr("e-mail") + ": Wladymyr1996@gmail.com" + "\n\n" +
					  tr("Version") + " - " + TData::getVersion() + "\n" +
					  tr("License - GNU General Public License v3.0") + "\n");

	setWindowTitle("Vypaluvach - " + tr("About program"));
}
