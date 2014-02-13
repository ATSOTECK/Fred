HEADERS       = sender.h\
                mouse_conversions.h
SOURCES       = sender.cpp \
                main.cpp
QT           += network widgets

# install
target.path = $$[QT_INSTALL_EXAMPLES]/network/multicastsender
INSTALLS += target

