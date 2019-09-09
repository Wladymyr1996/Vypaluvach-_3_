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

#ifndef TDATA_H
#define TDATA_H

#include <QWidget>
#include <QString>
#include <QList>
#include <QObject>
#include <QPixmap>
#include <QByteArray>
#include <QSettings>
#include <QApplication>
#include <QDesktopWidget>

/*
 * Клас сутність з даними
 * Дані зберігаються на комп'ютері в реєстрі чи конфігураційному файлі
 */

class TData : public QObject {
	Q_OBJECT

	public:
		TData(QWidget *mainwidget);

		static QString getVersion();
		QList<QString> getListHistoryFiles();

		int getMaxStepsCountX() const;
		void setMaxStepsCountX(int value);
		int getMaxStepsCountY() const;
		void setMaxStepsCountY(int value);
		int getMinPause() const;
		void setMinPause(int value);
		int getMaxPause() const;
		void setMaxPause(int value);
		int getSkippedRows() const;
		void setSkippedRows(int value);
		int getSkippedYStep() const;
		void setSkippedYStep(int value);
		int getHeatingTime() const;
		void setHeatingTime(int value);
		int getHeatigPlaceWidth() const;
		void setHeatigPlaceWidth(int value);
		int getImageWidthPlace() const;
		void setImageWidthPlace(int value);
		int getImageHeightPlace() const;
		void setImageHeightPlace(int value);
		bool getDoImageResize() const;
		void setDoImageResize(bool value);
		int getIndexMeasurementUnits() const;
		void setIndexMeasurementUnits(int value);
		qreal getCoefStepsPerMMbyX() const;
		void setCoefStepsPerMMbyX(const qreal &value);
		qreal getCoefStepsPerMMbyY() const;
		void setCoefStepsPerMMbyY(const qreal &value);
		int getBoudRate() const;
		void setBoudRate(int value);
		QString getLanguage() const;
		void setLanguage(const QString &value);

	private:
		QWidget *mainwidget;
		QSettings *dataFile;
		QList<QString> listHistoryFiles;
		int maxStepsCountX;
		int maxStepsCountY;
		int minPause;
		int maxPause;
		int skippedRows;
		int skippedYStep;
		int colorCount;
		int heatingTime;
		int heatigPlaceWidth;
		int imageWidthPlace;
		int imageHeightPlace;
		int indexMeasurementUnits;
		int boudRate;
		qreal coefStepsPerMMbyX;
		qreal coefStepsPerMMbyY;
		bool doImageResize = true;
		QString language;

	public slots:
		void openNewFile(QString filename);
		void saveAll();
		void saveWindowGeometry();

	signals:
		void updatedListHistoryFiles();
		void settingsUpdated();
};

extern TData *glData;

#endif // TDATA_H
