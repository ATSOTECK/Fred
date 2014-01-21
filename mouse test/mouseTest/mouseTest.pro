#-------------------------------------------------
#
# Project created by QtCreator 2014-01-17T20:24:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mouseTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    mouse3D/spwmath.h \
    mouse3D/spwmacro.h \
    mouse3D/spwerror.h \
    mouse3D/spwdata.h \
    mouse3D/siSyncPriv.h \
    mouse3D/siSyncDefines.h \
    mouse3D/siSync.h \
    mouse3D/siDefines.h \
    mouse3D/SiCfg.h \
    mouse3D/siapp.h \
    mouse3D/si.h \
    mouse3D/3DxWareVersionNumbers.h \
    mouse3D/stupidWindowsTypes.h

FORMS += mainwindow.ui

QMAKE_LIBDIR += C:\mouse3dlibs

LIBS += -lUser32 \
    -lsiapp \
    -lsiappD \
    -lsiappMT \
    -lsiappMTD \
    -lspwmath \
    -lspwmathD \
    -lspwmathMT \
    -lspwmathMTD
