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

#include "settingswindow.h"
#include "data.h"
#include <QIcon>
#include <QGroupBox>
#include <QComboBox>
#include <QDoubleValidator>
#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>
#include <QFormLayout>
#include <QPushButton>

TSettingsWindow::TSettingsWindow(QWidget *parent) {
	setWindowIcon(QIcon("://img/icon/setting.png"));
	this->parent = parent;

	appSettingBox = new QGroupBox(this);
	cncSettingBox = new QGroupBox(this);

	measureUnitComboBox = new QComboBox();
	measureUnitComboBox->addItem("step", QVariant(0));
	measureUnitComboBox->addItem("mm", QVariant(1));
	measureUnitComboBox->addItem("inch", QVariant(25.4f));

	boudRateComboBox = new QComboBox(this);
	boudRateComboBox->addItem("9600 bps", QVariant(9600));
	boudRateComboBox->addItem("19200 bps", QVariant(18200));
	boudRateComboBox->addItem("57600 bps", QVariant(57600));
	boudRateComboBox->addItem("115200 bps", QVariant(115200));
	boudRateComboBox->setCurrentText(QString::number(glData->getBoudRate()) + " bps");

	doubleValidator = new QDoubleValidator(0, 100000, 2, this);
	doubleValidator->setNotation(QDoubleValidator::StandardNotation);
	doubleValidator->setLocale(QLocale::system());
	QIntValidator *intValidator = new QIntValidator(this);
	intValidator->setBottom(0);

	resizeImageCheckBox = new QCheckBox(this);
	imageWidthLineEdit = new QLineEdit("0", this);
	imageWidthLineEdit->setValidator(doubleValidator);
	imageHeightLineEdit = new QLineEdit("0", this);
	imageHeightLineEdit->setValidator(doubleValidator);
	coefStepsPerMMbyXLineEdit = new QLineEdit("0", this);
	coefStepsPerMMbyXLineEdit->setValidator(doubleValidator);
	coefStepsPerMMbyYLineEdit = new QLineEdit("0", this);
	coefStepsPerMMbyYLineEdit->setValidator(doubleValidator);
	minPauseLineEdit = new QLineEdit("0", this);
	minPauseLineEdit->setValidator(intValidator);
	maxPauseLineEdit = new QLineEdit("0", this);
	maxPauseLineEdit->setValidator(intValidator);
	skippedRowsLineEdit = new QLineEdit("0", this);
	skippedRowsLineEdit->setValidator(intValidator);
	skippedYStepsLineEdit = new QLineEdit("0", this);
	skippedYStepsLineEdit->setValidator(intValidator);
	heatingTimeLineEdit = new QLineEdit("0", this);
	heatingTimeLineEdit->setValidator(intValidator);
	heatingPlaceWidthLineEdit = new QLineEdit("0", this);
	heatingPlaceWidthLineEdit->setValidator(intValidator);

	imageWidthLabel = new QLabel(this);
	imageHeightLabel = new QLabel(this);
	coefStepsPerMMbyXLabel = new QLabel(this);
	coefStepsPerMMbyYLabel = new QLabel(this);
	minPauseLabel = new QLabel(this);
	maxPauseLabel = new QLabel(this);
	skippedRowsLabel = new QLabel(this);
	skippedYStepsLabel = new QLabel(this);
	heatingTimeLabel = new QLabel(this);
	heatingPlaceWidthLabel = new QLabel(this);
	measureUnitLabel = new QLabel(this);
	boudRateLabel = new QLabel(this);

	QFormLayout *appFormLay = new QFormLayout(this);
	appFormLay->addRow(resizeImageCheckBox);
	appFormLay->addRow(imageWidthLabel, imageWidthLineEdit);
	appFormLay->addRow(imageHeightLabel, imageHeightLineEdit);
	appFormLay->addRow(measureUnitLabel, measureUnitComboBox);
	appFormLay->addRow(coefStepsPerMMbyXLabel, coefStepsPerMMbyXLineEdit);
	appFormLay->addRow(coefStepsPerMMbyYLabel, coefStepsPerMMbyYLineEdit);
	appSettingBox->setLayout(appFormLay);

	QFormLayout *cncFormLay = new QFormLayout(this);
	cncFormLay->addRow(minPauseLabel, minPauseLineEdit);
	cncFormLay->addRow(maxPauseLabel, maxPauseLineEdit);
	cncFormLay->addRow(skippedRowsLabel, skippedRowsLineEdit);
	cncFormLay->addRow(skippedYStepsLabel, skippedYStepsLineEdit);
	cncFormLay->addRow(heatingTimeLabel, heatingTimeLineEdit);
	cncFormLay->addRow(heatingPlaceWidthLabel, heatingPlaceWidthLineEdit);
	cncFormLay->addRow(boudRateLabel, boudRateComboBox);
	cncSettingBox->setLayout(cncFormLay);

	QHBoxLayout *buttonLay = new QHBoxLayout();
	cancelButton = new QPushButton(this);
	applyButton = new QPushButton(this);
	buttonLay->addWidget(cancelButton);
	buttonLay->addStretch();
	buttonLay->addWidget(applyButton);

	QHBoxLayout *editsLay = new QHBoxLayout();
	editsLay->addWidget(appSettingBox);
	editsLay->addWidget(cncSettingBox);

	QVBoxLayout *lay = new QVBoxLayout(this);
	lay->addLayout(editsLay);
	lay->addLayout(buttonLay);

	setLayout(lay);

	restore();
	updateLabels();

	setFixedSize(800, 500);
	setWindowFlags(Qt::WindowStaysOnTopHint);

	connect(resizeImageCheckBox, SIGNAL(toggled(bool)), imageWidthLineEdit, SLOT(setEnabled(bool)));
	connect(resizeImageCheckBox, SIGNAL(toggled(bool)), imageHeightLineEdit, SLOT(setEnabled(bool)));
	connect(resizeImageCheckBox, SIGNAL(toggled(bool)), measureUnitComboBox, SLOT(setEnabled(bool)));
	connect(resizeImageCheckBox, SIGNAL(toggled(bool)), SLOT(doUpdateMeasureUnits()));
	connect(measureUnitComboBox, SIGNAL(currentIndexChanged(int)), SLOT(doUpdateMeasureUnits()));
	connect(coefStepsPerMMbyXLineEdit, SIGNAL(textChanged(QString)), SLOT(doUpdateMeasureUnits()));
	connect(coefStepsPerMMbyYLineEdit, SIGNAL(textChanged(QString)), SLOT(doUpdateMeasureUnits()));
	connect(cancelButton, SIGNAL(clicked(bool)), SLOT(doCancelButton()));
	connect(applyButton, SIGNAL(clicked(bool)), SLOT(doApplyButton()));
}

void TSettingsWindow::updateLabels() {
	setWindowTitle(tr("Settings") + " — Vypaluvach");
	appSettingBox->setTitle(tr("App settings") + ":");
	cncSettingBox->setTitle(tr("CNC settings") + ":");
	measureUnitComboBox->setItemText(0, tr("step"));
	measureUnitComboBox->setItemText(1, tr("mm"));
	measureUnitComboBox->setItemText(2, tr("inch"));
	resizeImageCheckBox->setText(tr("Contain images in rectangle") + ":");
	imageWidthLabel->setText(tr("Image width") + " (" + measureUnitComboBox->currentText() + "):");
	imageHeightLabel->setText(tr("Image height") + " (" + measureUnitComboBox->currentText() + "):");
	coefStepsPerMMbyXLabel->setText(tr("Step per mm by X") + ":");
	coefStepsPerMMbyYLabel->setText(tr("Step per mm by Y") + ":");
	minPauseLabel->setText(tr("Minimal step interval (millisecond)") + ":");
	maxPauseLabel->setText(tr("Maximal step interval (millisecond)") + ":");
	skippedRowsLabel->setText(tr("Skipped imagerows by one row (pixel)") + ":");
	skippedYStepsLabel->setText(tr("Heght of row (steps)") + ":");
	heatingTimeLabel->setText(tr("Heating time (seconds)") + ":");
	heatingPlaceWidthLabel->setText(tr("Width of heating place (steps)") + ":");
	measureUnitLabel->setText(tr("Unit of measurement") + ":");
	boudRateLabel->setText(tr("Transfer speed") + ":");

	cancelButton->setText(tr("Cancel"));
	applyButton->setText(tr("Apply"));
}

void TSettingsWindow::restore() {
	resizeImageCheckBox->setChecked(glData->getDoImageResize());
	if (!resizeImageCheckBox->isChecked()) {
		imageHeightLineEdit->setEnabled(false);
		imageWidthLineEdit->setEnabled(false);
		measureUnitComboBox->setEnabled(false);
		coefStepsPerMMbyXLineEdit->setEnabled(false);
		coefStepsPerMMbyYLineEdit->setEnabled(false);
	}

	measureUnitComboBox->setCurrentIndex(glData->getIndexMeasurementUnits());
	coefStepsPerMMbyXLineEdit->setText(doubleValidator->locale().toString(glData->getCoefStepsPerMMbyX()));
	coefStepsPerMMbyYLineEdit->setText(doubleValidator->locale().toString(glData->getCoefStepsPerMMbyY()));

	minPauseLineEdit->setText(QString::number(glData->getMinPause()));
	maxPauseLineEdit->setText(QString::number(glData->getMaxPause()));
	skippedRowsLineEdit->setText(QString::number(glData->getSkippedRows()));
	skippedYStepsLineEdit->setText(QString::number((glData->getSkippedYStep())));
	heatingTimeLineEdit->setText(QString::number(glData->getHeatingTime()));
	heatingPlaceWidthLineEdit->setText(QString::number((glData->getHeatigPlaceWidth())));

	doUpdateMeasureUnits();
}

void TSettingsWindow::doUpdateMeasureUnits() {
	if (measureUnitComboBox->currentIndex() == 0) {
		imageWidthLineEdit->setText(doubleValidator->locale().toString(glData->getImageWidthPlace()));
		imageHeightLineEdit->setText(doubleValidator->locale().toString(glData->getImageHeightPlace()));
		coefStepsPerMMbyXLineEdit->setEnabled(false);
		coefStepsPerMMbyYLineEdit->setEnabled(false);
	} else {
		qreal coef = measureUnitComboBox->currentData().toReal();
		imageWidthLineEdit->setText(doubleValidator->locale().toString((glData->getImageWidthPlace() / doubleValidator->locale().toFloat(coefStepsPerMMbyXLineEdit->text())) / coef, 'f', 2));
		imageHeightLineEdit->setText(doubleValidator->locale().toString((glData->getImageHeightPlace() / doubleValidator->locale().toFloat(coefStepsPerMMbyYLineEdit->text())) / coef, 'f', 2));
		if (resizeImageCheckBox->isChecked()){
			coefStepsPerMMbyXLineEdit->setEnabled(true);
			coefStepsPerMMbyYLineEdit->setEnabled(true);
		} else {
			coefStepsPerMMbyXLineEdit->setEnabled(false);
			coefStepsPerMMbyYLineEdit->setEnabled(false);
		}
	}
	updateLabels();
}

void TSettingsWindow::doCancelButton() {
	restore();
	close();
}

void TSettingsWindow::doApplyButton() {
	doUpdate();
	glData->saveAll();
}

void TSettingsWindow::doUpdate() {
	glData->setDoImageResize(resizeImageCheckBox->isChecked());
	glData->setIndexMeasurementUnits(measureUnitComboBox->currentIndex());
	if (measureUnitComboBox->currentIndex() == 0) {
		glData->setImageWidthPlace(qRound(QLocale::system().toFloat(imageWidthLineEdit->text())));
		glData->setImageHeightPlace(qRound(QLocale::system().toFloat(imageHeightLineEdit->text())));
	} else {
		glData->setImageWidthPlace(qRound(QLocale::system().toFloat(imageWidthLineEdit->text()) * QLocale::system().toFloat(coefStepsPerMMbyXLineEdit->text()) * measureUnitComboBox->currentData().toFloat()));
		glData->setImageHeightPlace(qRound(QLocale::system().toFloat(imageHeightLineEdit->text()) * QLocale::system().toFloat(coefStepsPerMMbyYLineEdit->text()) * measureUnitComboBox->currentData().toFloat()));
	}
	glData->setCoefStepsPerMMbyX(QLocale::system().toFloat(coefStepsPerMMbyXLineEdit->text()));
	glData->setCoefStepsPerMMbyY(QLocale::system().toFloat(coefStepsPerMMbyYLineEdit->text()));

	glData->setMinPause(minPauseLineEdit->text().toInt());
	glData->setMaxPause(maxPauseLineEdit->text().toInt());
	glData->setSkippedRows(skippedRowsLineEdit->text().toInt());
	glData->setSkippedYStep(skippedYStepsLineEdit->text().toInt());
	glData->setHeatingTime(heatingTimeLineEdit->text().toInt());
	glData->setHeatigPlaceWidth(heatingPlaceWidthLineEdit->text().toInt());
	glData->setBoudRate(boudRateComboBox->currentData().toInt());
}

void TSettingsWindow::showEvent(QShowEvent *) {
	int x, y;
	x = parent->geometry().x()+(parent->geometry().width()-size().width())/2;
	y = parent->geometry().y()+(parent->geometry().height()-size().height())/2;
	setGeometry(x, y, size().width(), size().height());
	emit opened(true);
}

void TSettingsWindow::closeEvent(QCloseEvent *) {
	emit opened(false);
}
