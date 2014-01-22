#-------------------------------------------------
#
# Project created by QtCreator 2013-03-05T11:00:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Fred
TEMPLATE = app

DESTDIR = ../FredBin

MOC_DIR = ../FredBin/.moc
UI_DIR = ../FredBin/.uic
RCC_DIR = ../FredBin/.rcc
OBJECTS_DIR = ../FredBin/.obj

SOURCES += main.cpp\
        mainwindow.cpp \
    colorthresholddialog.cpp \
    histogramdialog.cpp \
    outlinedialog.cpp \
    squaredialog.cpp \
    aboutdialog.cpp \
    command.cpp \
    codeEditor/codeEditor.cpp \
    codeEditor/highlighter.cpp \
    codeEditor/miniMapC.cpp \
    codeEditor/searchWidget.cpp \
    projectItem.cpp \
    projectAction.cpp \
    cameraDialog.cpp \
    camera.cpp \
    fps.cpp

HEADERS  += mainwindow.h \
    colorthresholddialog.h \
    histogramdialog.h \
    outlinedialog.h \
    squaredialog.h \
    aboutdialog.h \
    command.h \
    codeEditor/codeEditor.h \
    codeEditor/highlighter.h \
    codeEditor/miniMapC.h \
    codeEditor/searchWidget.h \
    projectItem.h \
    projectAction.h \
    cameraDialog.h \
    camera.h \
    fps.h

FORMS    += mainwindow.ui \
    colorthresholddialog.ui \
    histogramdialog.ui \
    outlinedialog.ui \
    squaredialog.ui \
    aboutdialog.ui \
    cameraDialog.ui

RESOURCES += \
    res.qrc

DEPENDPATH += .

macx {
    INCLUDEPATH += /opt/local/include
    LIBS += -L/opt/local/lib/ -lopencv_core -lopencv_highgui -lopencv_imgproc
}

win32 {
    INCLUDEPATH += C:\\opencv\\build\\include
    LIBS += -LC:\\opencv\\build\\x86\\vc10\\lib \
        -lopencv_core246 \
        -lopencv_highgui246 \
        -lopencv_imgproc246 \
        -static-libgcc
}
