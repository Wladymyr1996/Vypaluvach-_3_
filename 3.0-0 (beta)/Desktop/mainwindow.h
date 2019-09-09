#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStatusBar>
#include <QLabel>
#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QFileDialog>
#include <QSignalMapper>
#include <QProgressBar>
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

#include <QMenuBar>
#include <QFileInfo>
#include <QInputDialog>
#include <QTranslator>

#include "data.h"
#include "imageviewer.h"
#include "commands.h"
#include "settingswindow.h"
#include "cnc.h"
#include "aboutwindow.h"

/*
 * Віджет головного вікна
 */

class MainWindow : public QMainWindow {
	Q_OBJECT

	public:
		MainWindow(QTranslator *translator, QWidget *parent = nullptr);
		~MainWindow();

	private:
		void makeMenus();
		void makeStatusBar();
		void updateTexts();

		QTranslator *translator;

		QMenu *fileMenu, *historyListMenu, *portMenu, *settingMenu, *controlMenu, *helpMenu, *langMenu;
		QAction *openImageAct, *openBinAct, *saveBinAct, *saveToCNCAct, *closeImageAct, *closeAppAct; //Menu "File"
		QAction *updatePortsAct;
		QAction *settingsAct, *settingsPortAct; //Menu "Settings"
		QAction *connectCNCAct, *setZeroAct, *beginPrintAct; //Menu "Control"
		QAction *getHelpAct, *getAboutProgramAct; //Menu "Help"
		QAction *setEnglishAct, *setUkrainianAct; //Menu "Language"
		QActionGroup *portListActGroup;
		QStatusBar *statusBar;
		QLabel *labelConectionStatus;
		QLabel *labelWorkedStatus;
		QProgressBar *progressBar;
		QSignalMapper *signalMapper, *portMapper, *langMapper;

		TImageViewer *imageViewer;
		TCommands *commandsLists;
		TSettingsWindow *settingWindow;
		TAboutWindow *aboutWindow;

		TCnc *cncPort;

	public slots:
		void doSetLang(QString lang);

	private slots:
		void doOpenImageAct(QString filename = QString(""));
		void doOpenBinAct(QString filename = QString(""));
		void doSaveBinAct();
		void doSaveToCNC();
		void doSettingsAct();
		void doOpenRecentAct(QString filename);
		void doUpdateHistoryList();
		void doUpdatePortList();
		void doConnectCNCAct();
		void doSetZeroAct();
		void doConnected(bool);
		void doBeginPrintAct();

		void closeFile();

		void doCommandListReady();
		void actionUpdate();
		void updateWorkedStatus();

	signals:
		void imageIsSeted(QString);
		void binIsSeted(QString);

	protected:
		void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
