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

#include "mainwindow.h"

MainWindow::MainWindow(QTranslator *translator, QWidget *parent) : QMainWindow(parent) {
	glData = new TData(this);
	this->translator = translator;

	imageViewer = new TImageViewer(this);
	signalMapper = new QSignalMapper(this);
	portMapper = new QSignalMapper(this);
	langMapper = new QSignalMapper(this);
	commandsLists = new TCommands(this);
	settingWindow = new TSettingsWindow(this);
	aboutWindow = new TAboutWindow(this);
	cncPort = new TCnc(this);

	makeMenus();
	makeStatusBar();

	setWindowIcon(QIcon("://img/icon.svg"));
	setCentralWidget(imageViewer);

	setMinimumSize(500, 400);

	updateTexts();

	connect(commandsLists, SIGNAL(commandListReady()), SLOT(doCommandListReady()));
	connect(this, SIGNAL(imageIsSeted(QString)), glData, SLOT(openNewFile(QString)));
	connect(this, SIGNAL(imageIsSeted(QString)), commandsLists, SLOT(doMakeLinesFromImage(QString)));
	connect(this, SIGNAL(binIsSeted(QString)), glData, SLOT(openNewFile(QString)));
	connect(this, SIGNAL(binIsSeted(QString)), commandsLists, SLOT(doMakeLinesFromBin(QString)));
	connect(glData, SIGNAL(updatedListHistoryFiles()), SLOT(doUpdateHistoryList()));
	connect(signalMapper, SIGNAL(mapped(QString)), SLOT(doOpenRecentAct(QString)));
	connect(portMapper, SIGNAL(mapped(QString)), cncPort, SLOT(setPort(QString)));
	connect(langMapper, SIGNAL(mapped(QString)), SLOT(doSetLang(QString)));

	connect(glData, SIGNAL(settingsUpdated()), SLOT(closeFile()));
	connect(cncPort, SIGNAL(connected(bool)), SLOT(doConnected(bool)));
	connect(cncPort, SIGNAL(setProgressMax(int)), progressBar, SLOT(setMaximum(int)));
	connect(cncPort, SIGNAL(setProgressValue(int)), progressBar, SLOT(setValue(int)));
	connect(cncPort, SIGNAL(setProgressValue(int)), SLOT(updateWorkedStatus()));

	connect(settingWindow, SIGNAL(opened(bool)), SLOT(setDisabled(bool)));
	connect(aboutWindow, SIGNAL(opened(bool)), SLOT(setDisabled(bool)));
}

MainWindow::~MainWindow() {

}

void MainWindow::makeMenus() {
	portListActGroup = new QActionGroup(this);

	{
		openImageAct = new QAction(this);
		openImageAct->setShortcut(QKeySequence("Ctrl+O"));
		openImageAct->setIcon(QIcon("://img/icon/open.png"));
		openBinAct = new QAction(this);
		openBinAct->setIcon(QIcon("://img/icon/openBin.png"));
		saveBinAct = new QAction(this);
		saveBinAct->setIcon(QIcon("://img/icon/save.png"));
		saveBinAct->setShortcut(QKeySequence("Ctrl+S"));
		saveBinAct->setEnabled(false);
		saveToCNCAct = new QAction(this);
		saveToCNCAct->setIcon(QIcon("://img/icon/saveToCNC.png"));
		saveToCNCAct->setEnabled(false);
		closeImageAct = new QAction(this);
		closeImageAct->setIcon(QIcon("://img/icon/closeImage.png"));
		closeAppAct = new QAction(this);
		closeAppAct->setIcon(QIcon("://img/icon/exit.png"));

		settingsAct = new QAction(this);
		settingsAct->setIcon(QIcon("://img/icon/setting.png"));

		connectCNCAct = new QAction(this);
		connectCNCAct->setIcon(QIcon("://img/icon/connect.png"));
		setZeroAct = new QAction(this);
		setZeroAct->setIcon(QIcon("://img/icon/setZero.png"));
		setZeroAct->setEnabled(false);
		beginPrintAct = new QAction(this);
		beginPrintAct->setIcon(QIcon("://img/icon/begin.png"));
		beginPrintAct->setEnabled(false);

		getHelpAct = new QAction(this);
		getHelpAct->setIcon(QIcon("://img/icon/help.png"));
		getHelpAct->setShortcut(QKeySequence("F1"));
		getAboutProgramAct = new QAction(this);
		getAboutProgramAct->setIcon(QIcon("://img/icon/help.png"));

		setEnglishAct = new QAction(QIcon("://img/icon/flags/us.png"), "English", this);
		setEnglishAct->setData("en");
		setUkrainianAct = new QAction(QIcon("://img/icon/flags/ua.png"), "Українська", this);
		setUkrainianAct->setData("uk");

		updatePortsAct = new QAction(this);
		updatePortsAct->setIcon(QIcon("://img/icon/update.png"));
	}

	{
		langMenu = new QMenu("Language", this);
		langMenu->setIcon(QIcon("://img/icon/lang.png"));
		langMenu->addAction(setEnglishAct);
		langMenu->addAction(setUkrainianAct);

		historyListMenu = new QMenu(this);
		historyListMenu->setIcon(QIcon("://img/icon/openHistory.png"));
		doUpdateHistoryList();

		portMenu = new QMenu(this);
		portMenu->setIcon(QIcon("://img/icon/settingPort.png"));
		doUpdatePortList();

		fileMenu = new QMenu(this);
		fileMenu->addAction(openImageAct);
		fileMenu->addAction(openBinAct);
		fileMenu->addMenu(historyListMenu);
		fileMenu->addSeparator();
		fileMenu->addAction(saveBinAct);
		fileMenu->addAction(saveToCNCAct);
		fileMenu->addSeparator();
		fileMenu->addAction(closeImageAct);
		fileMenu->addAction(closeAppAct);

		settingMenu = new QMenu(this);
		settingMenu->addAction(settingsAct);
		settingMenu->addMenu(langMenu);
		settingMenu->addSeparator();
		settingMenu->addMenu(portMenu);

		controlMenu = new QMenu(this);
		controlMenu->addAction(connectCNCAct);
		controlMenu->addSeparator();
		controlMenu->addAction(setZeroAct);
		controlMenu->addSeparator();
		controlMenu->addAction(beginPrintAct);

		helpMenu = new QMenu(this);
		helpMenu->addAction(getHelpAct);
		helpMenu->addSeparator();
		helpMenu->addAction(getAboutProgramAct);

	}

	QMenuBar *menu = new QMenuBar(this);
	menu->addMenu(fileMenu);
	menu->addMenu(settingMenu);
	menu->addMenu(controlMenu);
	menu->addMenu(helpMenu);
	setMenuBar(menu);

	connect(openImageAct, SIGNAL(triggered(bool)), SLOT(doOpenImageAct()));
	connect(openBinAct, SIGNAL(triggered(bool)), SLOT(doOpenBinAct()));
	connect(saveBinAct, SIGNAL(triggered(bool)), SLOT(doSaveBinAct()));
	connect(saveToCNCAct, SIGNAL(triggered(bool)), SLOT(doSaveToCNC()));
	connect(closeImageAct, SIGNAL(triggered(bool)), SLOT(closeFile()));
	connect(closeAppAct, SIGNAL(triggered(bool)), SLOT(close()));
	connect(settingsAct, SIGNAL(triggered(bool)), settingWindow, SLOT(show()));
	connect(getAboutProgramAct, SIGNAL(triggered(bool)), aboutWindow, SLOT(show()));
	connect(updatePortsAct, SIGNAL(triggered(bool)), SLOT(doUpdatePortList()));
	connect(connectCNCAct, SIGNAL(triggered(bool)), SLOT(doConnectCNCAct()));
	connect(beginPrintAct, SIGNAL(triggered(bool)), SLOT(doBeginPrintAct()));
	connect(setZeroAct, SIGNAL(triggered(bool)), SLOT(doSetZeroAct()));

	connect(setUkrainianAct, SIGNAL(triggered(bool)), langMapper, SLOT(map()));
	connect(setEnglishAct, SIGNAL(triggered(bool)), langMapper, SLOT(map()));
	langMapper->setMapping(setUkrainianAct, setUkrainianAct->data().toString());
	langMapper->setMapping(setEnglishAct, setEnglishAct->data().toString());
}

void MainWindow::makeStatusBar() {
	statusBar = new QStatusBar(this);
	labelConectionStatus = new QLabel(this);
	labelWorkedStatus = new QLabel(this);
	progressBar = new QProgressBar(this);

	statusBar->addWidget(labelConectionStatus);
	statusBar->addPermanentWidget(progressBar);
	statusBar->addPermanentWidget(labelWorkedStatus);
	//statusBar->set

	setStatusBar(statusBar);
}

void MainWindow::updateTexts() {

	//—| Top menu labels |———————————————————————————————————
	fileMenu->setTitle(tr("&File", "File menu in menubar"));
	openImageAct->setText(tr("Open &image file", "File menu in menubar"));
	openBinAct->setText(tr("Open &bin file", "File menu in menubar"));
	historyListMenu->setTitle(tr("Open recent"));
	saveBinAct->setText(tr("&Save bin file", "File menu in menubar"));
	saveToCNCAct->setText(tr("Save &to CNC", "File menu in menubar"));
	closeImageAct->setText(tr("Close image"));
	closeAppAct->setText(tr("&Close app", "File menu in menubar"));

	settingMenu->setTitle(tr("Settings", "Settings menu"));
	settingsAct->setText(tr("Settings", "Settings in menubar"));

	controlMenu->setTitle(tr("CNC control", "CNC control menu in menubar"));
	doConnected(false);
	setZeroAct->setText(tr("Set zero posititon"));
	beginPrintAct->setText(tr("Begin print"));

	helpMenu->setTitle(tr("Help"));
	getHelpAct->setText(tr("User's guide"));
	getAboutProgramAct->setText(tr("About"));

	portMenu->setTitle(tr("Select port"));
	updatePortsAct->setText(tr("Update port list"));

	//—| StatusBar labels |——————————————————————————————————
	labelConectionStatus->setText("CNC did't connected");
	updateWorkedStatus();


	//—| Window labels |—————————————————————————————————————
	setWindowTitle(QString("Vypaluvach ") + glData->getVersion());
}

void MainWindow::actionUpdate() {
	if (imageViewer->imageIsOpened()) {
		saveBinAct->setEnabled(true);
		saveToCNCAct->setEnabled(true);
		closeImageAct->setEnabled(true);
	} else {
		saveBinAct->setEnabled(false);
		saveToCNCAct->setEnabled(false);
		closeImageAct->setEnabled(false);
	}

	if (cncPort->isConnected()) {
		connectCNCAct->setText(tr("Disconnect from CNC"));
		setZeroAct->setEnabled(true);
		beginPrintAct->setEnabled(true);
	} else {
		connectCNCAct->setText(tr("Connect to CNC"));
		setZeroAct->setEnabled(false);
		beginPrintAct->setEnabled(false);
		saveToCNCAct->setEnabled(false);
	}
}

void MainWindow::updateWorkedStatus() {
	if (cncPort->isBusy()) {
		QString worked = "Transfer: ";
		worked += QString::number(progressBar->value()) + tr(" from ") + QString::number(progressBar->maximum());
		labelWorkedStatus->setText(worked);
	} else {
		labelWorkedStatus->setText(tr("I twiddling my thumbs"));
	}
}

void MainWindow::doOpenImageAct(QString filename)	{
	if (filename != "") {
		emit imageIsSeted(filename);
		return;
	}

	filename = QFileDialog::getOpenFileName(this, tr("Open image file"), "", tr("Images") + QString(" (*.jpg *.jpeg *.png *.gif *.bmp);;") + tr("All files") + QString(" (*.*)"));
	if (!filename.isNull()) {
		emit imageIsSeted(filename);
	}
}

void MainWindow::doCommandListReady() {
	imageViewer->setImage(commandsLists->getImage());
	imageViewer->setPrintingTime(commandsLists->getPrintingTime());

	actionUpdate();
}

void MainWindow::closeEvent(QCloseEvent *) {
	glData->saveWindowGeometry();
	cncPort->close();
}

void MainWindow::doOpenBinAct(QString filename) {
	if (filename != "") {
		emit binIsSeted(filename);
		return;
	}

	filename = QFileDialog::getOpenFileName(this, tr("Open binary file"), "", tr("BIN") + QString(" (*.bin);;") + tr("All files") + QString(" (*.*)"));
	if (!filename.isNull()) {
		emit binIsSeted(filename);
	}
}

void MainWindow::doSaveBinAct() {
	if (imageViewer->imageIsOpened()) {
		QString filename = QFileDialog::getSaveFileName(this, tr("Save binary file"), "", tr("BIN") + QString(" (*.bin);;"));
		if (filename == "") return;
		QFileInfo fileInfo(filename);
		if (fileInfo.suffix().toLower() != "bin")
			filename += ".bin";
		QFile file(filename);
		if (!file.open(QFile::WriteOnly)) {
			//qDebug() << "Can't open file";
			return;
		}
		file.write(commandsLists->getByteArray());
		file.close();
	}
}

void MainWindow::doSaveToCNC() {
	QString filename = QInputDialog::getText(this, tr("Enter name — Vypaluvach"), tr("Name (11 symbols): "));
	if (filename.length() > 11) filename = filename.remove(11, filename.length() - 11);
	filename += ".bin";
	QByteArray data;
	data.push_back(TCommands::TCommandType::beginFileName);
	data.push_back(filename.toLocal8Bit());
	data.push_back(TCommands::TCommandType::endFileName);
	data.push_back(commandsLists->getByteArray());
	data.push_back(TCommands::getByteArrayFromCommand(TCommands::getCommand(TCommands::TCommandType::endTransfering)));

	cncPort->beginTransfer(data);
}

void MainWindow::doSettingsAct() {
	setEnabled(false);
	settingWindow->show();
}

void MainWindow::doOpenRecentAct(QString filename) {
	QFileInfo file(filename);
	if (file.suffix().toLower() == "bin") doOpenBinAct(filename);
	else doOpenImageAct(filename);
}

void MainWindow::doUpdateHistoryList() {
	static QList<QAction*> actionList;
	for (int i = 0; i < actionList.count(); i++) {
		signalMapper->removeMappings(actionList.at(i));
		delete actionList.at(i);
	}
	actionList.clear();
	historyListMenu->clear();

	QList<QString> list = glData->getListHistoryFiles();
	for (int i = 0; i < list.count(); i++) {
		QFileInfo file(list[i]);
		QString filename = file.fileName();

		QAction *act = new QAction(filename, this);
		act->setData(list[i]);
		act->setToolTip(list[i]);
		act->setIcon(QIcon("://img/icon/file.png"));

		if (!file.exists()) act->setEnabled(false);

		actionList.push_back(act);
		historyListMenu->addAction(act);

		connect(act, SIGNAL(triggered(bool)), signalMapper, SLOT(map()));
		signalMapper->setMapping(act, act->data().toString());
	}

	if (historyListMenu->isEmpty()) historyListMenu->setEnabled(false);
	else historyListMenu->setEnabled(true);
}

void MainWindow::doUpdatePortList() {
	QList<QString> list = cncPort->getPortList();
	static QList<QAction*> actionList;
	for (int i = 0; i < actionList.count(); i++) {
		portMapper->removeMappings(actionList.at(i));
		delete actionList.at(i);
	}
	actionList.clear();
	portListActGroup->actions().clear();

	portMenu->clear();
	portMenu->addAction(updatePortsAct);
	portMenu->addSeparator();

	for (int i = 0; i < list.count(); i++) {
		QAction *act = new QAction(QIcon("://img/icon/port.png"), list.at(i), this);
		act->setCheckable(true);
		portMenu->addAction(act);
		portListActGroup->addAction(act);
		if (act->text() == cncPort->getPortName()) act->setChecked(true);
		connect(act, SIGNAL(triggered(bool)), portMapper, SLOT(map()));
		portMapper->setMapping(act, act->text());
		actionList.push_back(act);
	}
}

void MainWindow::doConnectCNCAct() {
	if (cncPort->isConnected()) {
		cncPort->close();
	} else {
		cncPort->open();
	}
}

void MainWindow::doSetZeroAct() {
	cncPort->sendCommand(TCommands::getCommand(TCommands::TCommandType::setAsZero));
}

void MainWindow::doConnected(bool) {
	actionUpdate();
}

void MainWindow::doBeginPrintAct() {
	cncPort->beginPrint(commandsLists->getByteArray());
}

void MainWindow::doSetLang(QString lang) {
	translator->load("://language/" + lang + ".qm");
	updateTexts();
	imageViewer->updateLabels();
	settingWindow->updateLabels();
	aboutWindow->updateLabels();

	glData->setLanguage(lang);
	glData->saveAll();
}

void MainWindow::closeFile() {
	imageViewer->closeImage();
	commandsLists->closeFile();

	actionUpdate();
}
