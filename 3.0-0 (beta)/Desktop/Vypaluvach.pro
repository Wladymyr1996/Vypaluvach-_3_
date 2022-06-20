#-------------------------------------------------
#
# VERSION 3.0-0
#
#-------------------------------------------------

#----------------------------------------------------------------------------
#    "Vypaluvach" - is control program for CNC wood burner "CNC Vypaluvach"
#    Copyright (C) 2019 Volodymyr Stadnyk
#    e-mail: Wladymyr1996@gmail.com
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <https://www.gnu.org/licenses/>
#----------------------------------------------------------------------------


QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

RC_FILE = icon.rc

CONFIG += c++11
TARGET = Vypaluvach
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS


SOURCES += aboutwindow.cpp \
    cnc.cpp \
    commands.cpp \
    data.cpp \
    imageviewer.cpp \
    main.cpp \
    mainwindow.cpp \
    settingswindow.cpp \


HEADERS  += aboutwindow.h \
    cnc.h \
    commands.h \
    data.h \
    imageviewer.h \
    mainwindow.h \
    settingswindow.h

TRANSLATIONS += language/uk.ts \
                language/en.ts

RESOURCES += \
    resources.qrc

DISTFILES += \
    language/uk.ts \
    language/en.ts
