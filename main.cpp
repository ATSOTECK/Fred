#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap pixmap(":/Images/loading.png");
    QSplashScreen splash(pixmap);
    splash.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::SplashScreen );
    splash.show();
    splash.showMessage("Getting cameras...");
    a.processEvents();
    MainWindow w;
    w.showMaximized();
    setMainWindow(&w);
    splash.finish(&w);

    return a.exec();
}
