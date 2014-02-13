HEADERS       = sender.h\
                mouse_conversions.h
SOURCES       = sender.cpp \
                main.cpp
QT           += network widgets

# install
target.path = $$[QT_INSTALL_EXAMPLES]/network/multicastsender
INSTALLS += target


DESTDIR = ../../FredBin

MOC_DIR = ../../FredBin/.mmoc
UI_DIR = ../../FredBin/.muic
RCC_DIR = ../../FredBin/.mrcc
OBJECTS_DIR = ../../FredBin/.mobj

