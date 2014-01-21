#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui/5.1.1/QtGui/qpa/qplatformnativeinterface.h>
#include <QMessageBox>
#include <QDebug>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

#include "mouse3D/spwmacro.h"  /* Common macros used by SpaceWare functions. */
#include "mouse3D/si.h"        /* Required for any SpaceWare support within an app.*/
#include "mouse3D/siapp.h"     /* Required for siapp.lib symbols */

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    int initMouse();
    
    //get mouse events
    int dispatchLoopNT();
    
    //handle mouse events
    void handleMotionEvent(SiSpwEvent *e);
    void handleZeroEvent();
    void handleButtonPressEvent(int button);
    void handleButtonReleaseEvent(int button);
    void handleDeviceChangeEvent(SiSpwEvent *e);

private:
    QWindow* windowForWidget(const QWidget* widget);
    HWND getHWNDForWidget(const QWidget* widget);
    
    SiHdl mDevHdl;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
