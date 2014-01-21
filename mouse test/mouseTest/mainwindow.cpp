#include "mainwindow.h"
#include "ui_mainwindow.h"

QWindow *MainWindow::windowForWidget(const QWidget* widget) {
    QWindow* window = widget->windowHandle();
    if (window)
        return window;
    const QWidget* nativeParent = widget->nativeParentWidget();
    if (nativeParent)
        return nativeParent->windowHandle();
    return 0;
}

HWND MainWindow::getHWNDForWidget(const QWidget* widget) {
    QWindow* window = windowForWidget(widget);
    if (window) {
        QPlatformNativeInterface *qinterface = QGuiApplication::platformNativeInterface();
        return static_cast<HWND>(qinterface->nativeResourceForWindow(QByteArrayLiteral("handle"), window));
    }
    
    return 0;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if (!initMouse()) {
        QMessageBox::warning(this, tr("ERROR!"), tr("Mouse init failed!\nNo mouse!"));
        return;
    }
}

MainWindow::~MainWindow() {
    delete ui;
}

int MainWindow::initMouse() {
    HWND hwnd = getHWNDForWidget(this);
    SiOpenData data;
    
    if (SiInitialize() == SPW_DLL_LOAD_ERROR) {
        QMessageBox::warning(this, "ERROR", "No DLL");
    }
    
    SiOpenWinInit(&data, hwnd);
    SiSetUiMode(mDevHdl, SI_UI_ALL_CONTROLS);
    
    if ((mDevHdl = SiOpen("3DTest", SI_ANY_DEVICE, SI_NO_MASK, SI_NOT_EVENT, &data)) == NULL) {
        //Will fail if not plugged in.
        SiTerminate();
        return 0;
    } else {
        return 1;
    }
    
    return dispatchLoopNT();
}

int MainWindow::dispatchLoopNT() {
    int button;
    MSG msg;
    BOOL handled;
    SiSpwEvent event;
    SiGetEventData eventData;
    
    while (GetMessage(&msg, NULL, 0, 0)) {
        handled = SPW_FALSE;
        
        SiGetEventWinInit(&eventData, msg.message, msg.wParam, msg.lParam);
        
        if (SiGetEvent(mDevHdl, SI_AVERAGE_EVENTS, &eventData, &event) == SI_IS_EVENT) {
            if (event.type == SI_MOTION_EVENT) {
                handleMotionEvent(&event);
            }
            
            handled = SPW_TRUE;
        }
        
        //not mouse event
        if (handled == SPW_FALSE) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    
    return 0;
}

void MainWindow::handleMotionEvent(SiSpwEvent *e) {
    qDebug() << "Motion handled!";
    
    char buff0[30];
    char buff1[30];
    char buff2[30];
    char buff3[30];
    char buff4[30];
    char buff5[30];
    char buff6[30];
    
    int len0, len1, len2, len3, len4, len5, len6;
    
    len0 = _stprintf(buff0, "TX: %d", e->u.spwData.mData[SI_TX]);
    len1 = _stprintf(buff1, "TY: %d", e->u.spwData.mData[SI_TY]);
    len2 = _stprintf(buff2, "TZ: %d", e->u.spwData.mData[SI_TZ]);
    len3 = _stprintf(buff3, "RX: %d", e->u.spwData.mData[SI_RX]);
    len4 = _stprintf(buff4, "RY: %d", e->u.spwData.mData[SI_RY]);
    len5 = _stprintf(buff5, "RZ: %d", e->u.spwData.mData[SI_RZ]);
    len6 = _stprintf(buff6, " P: %d", e->u.spwData.period);
}


