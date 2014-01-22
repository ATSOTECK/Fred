#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.setHWND(getHWNDForWidget(&w));
    w.show();

    return a.exec();
}
