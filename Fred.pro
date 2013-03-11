#-------------------------------------------------
#
# Project created by QtCreator 2013-03-05T11:00:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Fred
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    colorthresholddialog.cpp \
    histogramdialog.cpp \
    outlinedialog.cpp \
    squaredialog.cpp

HEADERS  += mainwindow.h \
    colorthresholddialog.h \
    histogramdialog.h \
    outlinedialog.h \
    squaredialog.h

FORMS    += mainwindow.ui \
    colorthresholddialog.ui \
    histogramdialog.ui \
    outlinedialog.ui \
    squaredialog.ui

RESOURCES += \
    res.qrc

DEPENDPATH += .
INCLUDEPATH += /opt/local/include
LIBS += -L/opt/local/lib/ -lopencv_core -lopencv_highgui -lopencv_imgproc
