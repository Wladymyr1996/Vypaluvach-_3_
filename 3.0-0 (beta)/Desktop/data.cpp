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

#include "data.h"

#define VERSION "3.0-0"

TData::TData(QWidget *mainwidget) {
	this->mainwidget = mainwidget;
	dataFile = new QSettings("ShaiTech", "Vypaluvach");

	dataFile->beginGroup("/Window");
		mainwidget->setGeometry(dataFile->value("/Geometry", QRect((QApplication::desktop()->geometry().width() - 500) / 2,
																   (QApplication::desktop()->geometry().height() - 400) / 2,
																   500, 400)).toRect());
		QList<QVariant> list = dataFile->value("/OldFiles", QVariant(QList<QString>())).toList();
		for (int i = 0; i < list.count(); i++ ) {
			listHistoryFiles.push_back(list.at(i).toString());
		}
	dataFile->endGroup();

	dataFile->beginGroup("/AppSettings");
		setDoImageResize(dataFile->value("/ContainImage", false).toBool());
		setImageWidthPlace(dataFile->value("/Width", 500).toInt());
		setImageHeightPlace(dataFile->value("/Height", 500).toInt());
		setIndexMeasurementUnits(dataFile->value("/MeasureUnits", 0).toInt());
		setCoefStepsPerMMbyX(dataFile->value("/StepPerMMbyX", 1).toFloat());
		setCoefStepsPerMMbyY(dataFile->value("/StepPerMMbyY", 1).toFloat());
		setLanguage(dataFile->value("/Language", "en").toString());
	dataFile->endGroup();

	dataFile->beginGroup("/CncSettings");
		setMinPause(dataFile->value("/MinStepsIntervalByX", 3).toInt());
		setMaxPause(dataFile->value("/MaxStepsIntervalByX", 32).toInt());
		setSkippedRows(dataFile->value("/SkippedRowsByOneLine", 1).toInt());
		setSkippedYStep(dataFile->value("/StepsNumberByY", 15).toInt());
		setHeatingTime(dataFile->value("/HeatingTime", 30).toInt());
		setHeatigPlaceWidth(dataFile->value("/HeatingPlaceWidth", 50).toInt());
		setBoudRate(dataFile->value("/BaudRate", 9600).toInt());
	dataFile->endGroup();
}

QString TData::getVersion() {
	return VERSION;
}

QList<QString> TData::getListHistoryFiles() {
	return listHistoryFiles;
}

int TData::getMaxStepsCountX() const
{
	return maxStepsCountX;
}

void TData::setMaxStepsCountX(int value)
{
	maxStepsCountX = value;
}

int TData::getMaxStepsCountY() const
{
	return maxStepsCountY;
}

void TData::setMaxStepsCountY(int value)
{
	maxStepsCountY = value;
}

int TData::getMinPause() const
{
	return minPause;
}

void TData::setMinPause(int value)
{
	minPause = value;
}

int TData::getMaxPause() const
{
	return maxPause;
}

void TData::setMaxPause(int value)
{
	maxPause = value;
}

int TData::getSkippedRows() const
{
	return skippedRows;
}

void TData::setSkippedRows(int value)
{
	skippedRows = value;
}

int TData::getSkippedYStep() const
{
	return skippedYStep;
}

void TData::setSkippedYStep(int value)
{
	skippedYStep = value;
}

int TData::getHeatingTime() const
{
	return heatingTime;
}

void TData::setHeatingTime(int value)
{
	heatingTime = value;
}

int TData::getHeatigPlaceWidth() const
{
	return heatigPlaceWidth;
}

void TData::setHeatigPlaceWidth(int value)
{
	heatigPlaceWidth = value;
}

int TData::getImageWidthPlace() const
{
	return imageWidthPlace;
}

void TData::setImageWidthPlace(int value)
{
	imageWidthPlace = value;
}

int TData::getImageHeightPlace() const
{
	return imageHeightPlace;
}

void TData::setImageHeightPlace(int value)
{
	imageHeightPlace = value;
}

bool TData::getDoImageResize() const
{
	return doImageResize;
}

void TData::setDoImageResize(bool value)
{
	doImageResize = value;
}

int TData::getIndexMeasurementUnits() const
{
	return indexMeasurementUnits;
}

void TData::setIndexMeasurementUnits(int value)
{
	indexMeasurementUnits = value;
}

qreal TData::getCoefStepsPerMMbyX() const
{
	return coefStepsPerMMbyX;
}

void TData::setCoefStepsPerMMbyX(const qreal &value)
{
	coefStepsPerMMbyX = value;
}

qreal TData::getCoefStepsPerMMbyY() const
{
	return coefStepsPerMMbyY;
}

void TData::setCoefStepsPerMMbyY(const qreal &value)
{
	coefStepsPerMMbyY = value;
}

int TData::getBoudRate() const
{
	return boudRate;
}

void TData::setBoudRate(int value)
{
	boudRate = value;
}

QString TData::getLanguage() const
{
	return language;
}

void TData::setLanguage(const QString &value)
{
	language = value;
}

void TData::openNewFile(QString filename) {
	listHistoryFiles.push_front(filename);
	for (int i = 1; i < listHistoryFiles.count(); i++) {
		if (filename == listHistoryFiles[i]) listHistoryFiles.removeAt(i);
	}
	while (listHistoryFiles.count() > 5) {
		listHistoryFiles.pop_back();
	}

	emit updatedListHistoryFiles();
}

void TData::saveAll() {
	QList<QVariant> list;
	for (int i = 0; i < listHistoryFiles.count(); i++ ) {
		list.push_back(listHistoryFiles.at(i));
	}

	dataFile->beginGroup("/Window");
		dataFile->setValue("/Geometry", mainwidget->geometry());
		dataFile->setValue("/OldFiles", list);
	dataFile->endGroup();

	dataFile->beginGroup("/AppSettings");
		dataFile->setValue("/ContainImage", getDoImageResize());
		dataFile->setValue("/Width", getImageWidthPlace());
		dataFile->setValue("/Height", getImageHeightPlace());
		dataFile->setValue("/MeasureUnits", getIndexMeasurementUnits());
		dataFile->setValue("/StepPerMMbyX", getCoefStepsPerMMbyX());
		dataFile->setValue("/StepPerMMbyY", getCoefStepsPerMMbyY());
		dataFile->setValue("/Language", getLanguage());
	dataFile->endGroup();

	dataFile->beginGroup("/CncSettings");
		dataFile->setValue("/MinStepsIntervalByX", getMinPause());
		dataFile->setValue("/MaxStepsIntervalByX", getMaxPause());
		dataFile->setValue("/SkippedRowsByOneLine", getSkippedRows());
		dataFile->setValue("/StepsNumberByY", getSkippedYStep());
		dataFile->setValue("/HeatingTime", getHeatingTime());
		dataFile->setValue("/HeatingPlaceWidth", getHeatigPlaceWidth());
		dataFile->setValue("/BaudRate", getBoudRate());
	dataFile->endGroup();

	emit settingsUpdated();
}

void TData::saveWindowGeometry() {
	QList<QVariant> list;
	for (int i = 0; i < listHistoryFiles.count(); i++ ) {
		list.push_back(listHistoryFiles.at(i));
	}

	dataFile->beginGroup("/Window");
		dataFile->setValue("/Geometry", mainwidget->geometry());
		dataFile->setValue("/OldFiles", list);
	dataFile->endGroup();
}

TData *glData;
