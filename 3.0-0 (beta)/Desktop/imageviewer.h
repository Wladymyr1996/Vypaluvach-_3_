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

#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTime>

/*
 * головний віджет, який відображає зображення
 */

class TImageViewer : public QWidget{
	Q_OBJECT

	public:
		explicit TImageViewer(QWidget *parent = nullptr);
		bool imageIsOpened();

	private:
		QString labelNoImage;
		QString labelPrintingTime;
		uint64_t printingTime;
		QPixmap *picture = NULL;
		bool isOpened = false;
		qreal scale;
		QRect imgFrame;

	private:
		void updateImagePos();

	public slots:
		void setPrintingTime(uint64_t time);
		void updateLabels();
		void setImage(QPixmap filename);
		void closeImage();

	protected:
		void paintEvent(QPaintEvent *event);
		void resizeEvent(QResizeEvent *event);
};

#endif // IMAGEVIEWER_H
