#-------------------------------------------------
#
# Project created by QtCreator 2013-03-02T18:55:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Fred
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Camera.cpp

HEADERS  += mainwindow.h \
    Camera.h \
    videoSettings.h

FORMS    += mainwindow.ui
