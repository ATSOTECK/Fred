#include "mainwindow.h"
#include "ui_mainwindow.h"

static QWindow *windowForWidget(const QWidget* widget) {
    QWindow* window = widget->windowHandle();
    if (window)
        return window;
    const QWidget* nativeParent = widget->nativeParentWidget();
    if (nativeParent)
        return nativeParent->windowHandle();
    return 0;
}

HWND getHWNDForWidget(const QWidget* widget) {
    QWindow* window = ::windowForWidget(widget);
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
    
    //int i

    if (!initMouse()) {
        return;
    }
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setHWND(HWND hwnd) {
    mhwnd = hwnd;
}

int MainWindow::initMouse() {
    if (mhwnd == 0) {
        QMessageBox::warning(this, tr("ERROR!"), tr("Unable to get window handle!"));
        return 0;
    }
    
    SiOpenData data;
    
    SpwRetVal val;
    
    //0 is no error
    val = SiInitialize();
    if ((int)val > 0) {
        QMessageBox::warning(this, "ERROR", "ERROR " + QString::number((int)val));
        return 0;
    }
    
    SiOpenWinInit(&data, mhwnd);
    SiSetUiMode(mDevHdl, SI_UI_ALL_CONTROLS);
    
    if ((mDevHdl = SiOpen("mouseTest", SI_ANY_DEVICE, SI_NO_MASK, SI_EVENT, &data)) == NULL) {
        //Will fail if not plugged in.
        QMessageBox::warning(this, tr("ERROR!"), tr("Mouse init failed!\nNo mouse!"));
        SiTerminate();
        return 0;
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
    
    len0 = sprintf_s(buff0, "TX: %d", e->u.spwData.mData[SI_TX]);
    len1 = sprintf_s(buff1, "TY: %d", e->u.spwData.mData[SI_TY]);
    len2 = sprintf_s(buff2, "TZ: %d", e->u.spwData.mData[SI_TZ]);
    len3 = sprintf_s(buff3, "RX: %d", e->u.spwData.mData[SI_RX]);
    len4 = sprintf_s(buff4, "RY: %d", e->u.spwData.mData[SI_RY]);
    len5 = sprintf_s(buff5, "RZ: %d", e->u.spwData.mData[SI_RZ]);
    len6 = sprintf_s(buff6, " P: %d", e->u.spwData.period);
    
    ui->lblTX->setText("TX: " + QString::number(len0));
}


