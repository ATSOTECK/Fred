#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "codeEditor/codeEditor.h"
#include "codeEditor/searchWidget.h"
#include "cameraDialog.h"

#include <QtCore>
#include <QStringListModel>

#include <QDebug>
#include <iostream>


MainWindow *w;

void setMainWindow(MainWindow *mw) {
    w = mw;
}

void catchMessage(QtMsgType type, const QMessageLogContext &, const QString &msg) {
    switch (type) {
    case QtDebugMsg:
        w->debug(msg);
        break;
    case QtWarningMsg:
        w->warn(msg);
        break;
    case QtCriticalMsg:
        w->crit(msg);
        break;
    case QtFatalMsg:
        w->fail(msg);
        break;
    default:
        break;
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mKernelSize(3),
    mStatusLabel(new QLabel),
    mSearchWidget(new SearchWidget(0, this)),
    mSearchWidgetAdded(false),
    mIsRunning(false),
    mPoped(false),
    mFPS(new FPS()),
    mFPSTimer(new QTimer(this)),
    mFPSLabel(new QLabel("0", this))
{
    ui->setupUi(this);
    
    qInstallMessageHandler(catchMessage);
    
    setCentralWidget(ui->tabs);
    
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
    setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    
    setDockNestingEnabled(true);

    mBlack = QColor("black");
    mBlue = QColor("blue");
    mRed = QColor("red");
    mGreen = QColor("green");
    
    ui->console->setReadOnly(true);
    ui->consoleDockWidgetContents->setContentsMargins(0, 0, 0, 0);
    ui->commandsDockWidgetContents->setContentsMargins(0, 0, 0, 0);
    ui->commands->setAnimated(true);
    
    debug("Starting...");
    QString version = CV_VERSION;
    debug("OpenCV version " + version);

    //ui->actionStart->setDisabled(true);
    ui->actionPause->setDisabled(true);

    mNcams = getCamCount();
    debug(QString::number(mNcams) + " camera(s) detected.");
    createCameras();

    QMenu *devicesMenu = new QMenu();
    for (int i = 0; i < mNcams; i++) {
        ProjectAction *testAction = new ProjectAction(QString::number(i), this);
        testAction->setIndex(i);
        //devicesMenu->addAction(testAction);
        ui->menuDevices->addAction(testAction);
    }
    
    //connect(devicesMenu, SIGNAL(triggered(QAction*)), 
            //this, SLOT(getCam(QAction*)));
    
    connect(ui->menuDevices, SIGNAL(triggered(QAction*)), 
            this, SLOT(getCam(QAction*)));
    connect(ui->actionRefresh,SIGNAL(triggered()),this,SLOT(refresh()));
    
    //QAction *testAction = new QAction("test item", this);
    //QAction *testAction1 = new QAction("another test item", this);
    //devicesMenu->addAction(testAction);
    //devicesMenu->addAction(testAction1);

    QToolButton *devicesButton = new QToolButton();
    devicesButton->setIcon(QIcon(":/Images/webcam.png"));
    devicesButton->setMenu(devicesMenu);
    devicesButton->setPopupMode(QToolButton::InstantPopup);
    devicesButton->setToolTip("Available devices");
    //ui->mainToolBar->addWidget(devicesButton);
    
    connect(ui->actionPause, SIGNAL(triggered()), this, SLOT(pauseButtonClicked()));
    connect(ui->actionStart, SIGNAL(triggered()), this, SLOT(pauseButtonClicked()));

    connect(ui->actionThreshold, SIGNAL(triggered()), this, SLOT(thresholdClicked()));
    connect(ui->actionHistogram, SIGNAL(triggered()), this, SLOT(showHistogramClicked()));
    connect(ui->actionOutline, SIGNAL(triggered()), this, SLOT(showOutlineClicked()));
    connect(ui->actionSquares, SIGNAL(triggered()), this, SLOT(showSquaresClicked()));
    connect(ui->actionAboutFred, SIGNAL(triggered()), this, SLOT(aboutDialogClicked()));
    connect(ui->actionClear_console, SIGNAL(triggered()), this, SLOT(clearConsoleClicked()));

    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(load()));

    mThresholdDiablog = new ColorThresholdDialog(this);
    mHistogramDialog = new HistogramDialog(this);
    mOutlineDialog = new OutlineDialog(this);
    mSquaresDialog = new SquareDialog(this);

    connect(mThresholdDiablog, SIGNAL(updateMainWindowTreshold()), this, SLOT(updateThreshold()));
    connect(ui->actionToolbar, SIGNAL(triggered()), this, SLOT(showToolbarClicked()));
    ui->actionToolbar->setChecked(true);

    //ui->commandsDock->close();
    //ui->originalImage->close();

    ui->originalImage->setScaledContents(true);
    ui->processedImage->setScaledContents(true);

    setUpActions();
    setUpCommandDock();
    setUpCommands();

    connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    rMin = gMin = bMin = 0;
    rMax = gMax = bMax = 255;
    
    mMainCodeEditor = addCodeEditor();
    connect(mMainCodeEditor, SIGNAL(statusInfoChanged(QString)), this, SLOT(updateStatusLabel(QString)));
    
    ui->tabs->addTab(mMainCodeEditor, "main.lua");
    
    connect(mSearchWidget, SIGNAL(closeButtonClicked()), this, SLOT(closeFind()));
    mSearchWidget->setVisible(false);
    mSearchWidget->setCodeEditor(mMainCodeEditor);
    
    mStatusLabel->setText("FPS: ");
    ui->statusBar->addWidget(mStatusLabel);
    
    ui->commands->setAttribute(Qt::WA_MacShowFocusRect, false);

    mTimerTime = 16;
    
    Command<MainWindow> c("Find Circle", *this, &MainWindow::doCircles);
    addCommand(c);

    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(processFrameAndUpdateGUI()));
    connect(mFPSTimer, SIGNAL(timeout()), this, SLOT(displayFPS()));
    //timer->start(timerTime);
    ui->statusBar->addWidget(mFPSLabel);
    
    //ui->tabs->addTab(ui->originalImage, "a new tab");
    
    ui->originalImage->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->originalImage, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(getCamOneContextMenu(QPoint)));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::debug(const QString &msg) {
    QColor c(98, 217, 239);
    ui->console->setTextColor(c);
    ui->console->append(msg);
    ui->console->setTextColor(Qt::white);
}
void MainWindow::warn(const QString &msg) {
    ui->console->setTextColor(QColor(230, 219, 116));
    ui->console->append(msg);
    ui->console->setTextColor(Qt::white);
}

void MainWindow::crit(const QString &msg) {
    ui->console->setTextColor(Qt::red);
    ui->console->append(msg);
    ui->console->setTextColor(Qt::white);
}

void MainWindow::fail(const QString &msg) {
    ui->console->setTextColor(Qt::red);
    ui->console->append(msg);
    ui->console->setTextColor(Qt::white);
}

ProjectItem *MainWindow::addRoot(QString name, ProjectItem::Type type) {
    ProjectItem *itm = new ProjectItem(ui->commands);
    itm->setText(0, name);
    itm->setType(type);

    return itm;
}

void MainWindow::addChild(ProjectItem *parent, ProjectItem::Type type, QString path, QString name, QIcon &icon) {
    ProjectItem *itm = new ProjectItem();
    itm->setText(0, name);
    itm->setIcon(0, icon);
    itm->setType(type);
    itm->setPath(path);
    parent->addChild(itm);
    //TODO: check preferences
    parent->sortChildren(0, Qt::AscendingOrder);
}

void MainWindow::addChild(ProjectItem *parent, ProjectItem *child) {
    parent->addChild(child);
}

void MainWindow::setCamera(int c, int w, int h) {
    /*
    if (mCamera.isOpened())
        mCamera.release();
    
    mCamera.open(c);
    mCamera.set(CV_CAP_PROP_FRAME_WIDTH, w);
    mCamera.set(CV_CAP_PROP_FRAME_HEIGHT, h);
    
    debug("Camera set successful!");
    */
}

void MainWindow::getCam(QAction *c) {
    int n = c->text().toInt();
    setCamera(n, 640, 480);
}

void MainWindow::addCameraDialog(QAction *c) {
    if (c->text() != "Pop") {
            /*
            int n = c->text().toInt();
            this->pauseButtonClicked();
            setCamera(1,n);
            this->pauseButtonClicked();
            */
            return;
        }
        
        CameraDialog *dialog = new CameraDialog(mCameras.at(0) ,this);
        dialog->setLabelPixmap();
        mPoped = true;
        dialog->exec();
}

void MainWindow::setUpCommandDock() {

}

void MainWindow::setUpCommands() {
    ui->commands->setColumnCount(1);
    ui->commands->setHeaderLabel("Commands");
    
    mRoot = addRoot("Running Commands", ProjectItem::CommandRoot);
    mRoot->setData(0, Qt::UserRole, ROOT);
    mRoot->setExpanded(true);

    ProjectItem *drawOriginalImageItem = new ProjectItem();
    drawOriginalImageItem->setText(0, "Draw original image");
    drawOriginalImageItem->setData(0, Qt::UserRole, ORIGINAL);

    addChild(mRoot, drawOriginalImageItem);

    ui->commands->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->commands, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(getContextMenu(QPoint)));
}



void MainWindow::setUpActions() {
    mPauseAllCommandsAction = new QAction(tr("&Pause All Commands"), this);
    mResumeAllCommandsAction = new QAction(tr("&Resume All Commands"), this);
    mResumeAllCommandsAction->setEnabled(false);

    mPauseSelectedCommandAction = new QAction(tr("&Pause Command"), this);
    mResumeSelectedCommandAction = new QAction(tr("&Resume Command"), this);
    mResumeSelectedCommandAction->setEnabled(false);

    mDeleteSelectedCommandAction = new QAction(tr("&Delete Command"), this);
    
    mHideConsoleAction = new QAction(tr("&Hide Console"), this);
    mHideConsoleAction->setShortcut(QKeySequence("Escape"));
    
    ui->menuView->addAction(mHideConsoleAction);
    
    connect(mHideConsoleAction, SIGNAL(triggered()), this, SLOT(hideConsole()));
    
    connect(ui->actionFind, SIGNAL(triggered()), this, SLOT(find()));
}

#ifdef Q_OS_WIN
int MainWindow::getCamCount() {
    CvCapture *cap;
    int ncams = 0;
    while (ncams < 6) {
        cap = cvCreateCameraCapture(ncams++);
        if (cap == NULL)
            break;
        cvReleaseCapture(&cap);
    }

    cvReleaseCapture(&cap);
    return (ncams - 1);
}
#endif

#ifdef Q_OS_MACX
int MainWindow::getCamCount() {
    CvCapture *cap;
    int ncams = 0;
    while (true) {
        cap = cvCreateCameraCapture(ncams++);
        if (cap == NULL)
            break;
        cvReleaseCapture(&cap);
        std::cout << "cam" << ncams << std::endl;
    }

    //cvReleaseCapture(&cap);
    return (ncams - 1);
}
#endif

void MainWindow::createCameras() {
    ///*
    for (int i = 0; i < mNcams; i++) {
        Camera *c = new Camera(i, this);
        std::cout << "here in createCameras " << i << std::endl;
        mCameras.append(c);
    }
    //*/
    /*
    Camera *c = new Camera(0, this);
    mCameras.append(c);
    Camera *c1 = new Camera(1, this);
    mCameras.append(c1);
    */
    
    std::cout << "returning from createCameras\n";
    
    //error has occured
}

void MainWindow::getContextMenu(const QPoint &point) {
    QTreeWidgetItem *itm = ui->commands->itemAt(point);
    if (!itm)
        return;

    QMenu *menu = new QMenu(ui->commands);

    switch (itm->data(0, Qt::UserRole).toInt()) {
    case ROOT:
        menu->addAction(mPauseAllCommandsAction);
        menu->addAction(mResumeAllCommandsAction);
        menu->exec(ui->commands->viewport()->mapToGlobal(point));
        break;
    case ORIGINAL:
        menu->addAction(mPauseSelectedCommandAction);
        menu->addAction(mResumeSelectedCommandAction);
        menu->addSeparator();
        menu->addAction(mDeleteSelectedCommandAction);
        menu->exec(ui->commands->viewport()->mapToGlobal(point));
        break;
    default:
        break;
    }
}

void MainWindow::getCamOneContextMenu(const QPoint &point) {
    mCameraMenu = new QMenu(ui->originalImage);
    QAction *action = new QAction("1", 0);
    QAction *action2 = new QAction("2", 0);
    QAction *action3 = new QAction("3", 0);
    
    QAction *popOutAction = new QAction("Pop", mCameraMenu);
    
    mCameraMenu->addAction(action);
    mCameraMenu->addAction(action2);
    mCameraMenu->addAction(action3);
    mCameraMenu->addSeparator();
    mCameraMenu->addAction(popOutAction);
    
    connect(mCameraMenu, SIGNAL(triggered(QAction*)),
            this, SLOT(addCameraDialog(QAction*)));
    
    mCameraMenu->dumpObjectInfo();
    
    mCameraMenu->exec(ui->originalImage->mapToGlobal(point));
    
}

CodeEditor *MainWindow::addCodeEditor() {
    Highlighter *highlighter = 0;
    QCompleter *completer;
    CodeEditor *codeEditor = new CodeEditor("main", 0, highlighter);
    codeEditor->setUndoRedoEnabled(true);
    codeEditor->setTabStopWidth(32);
#ifdef Q_OS_MAC
    int size = 12;
    QFont font("Monaco", size);
#endif
#ifdef Q_OS_WIN
    int size = 10;
    QFont font("Consolas", size);
#endif
#ifdef Q_OS_LINUX
    int size = 10;
    QFont font("Inconsolata-g", size);
#endif
    codeEditor->setFont(font);
    codeEditor->appendPlainText("--main.lua\n");
    codeEditor->appendPlainText("function update()\n\tif not paused() then\n\t\tshowOriginalImage(0)\n\tend\nend");
    highlighter = new Highlighter(codeEditor->document());
    completer = new QCompleter();
    completer->setModel(modelFromFile(completer, ":/wordlist.txt"));
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setWrapAround(false);
    completer->popup()->setStyleSheet("color: #848484; background-color: #2E2E2E; selection-background-color: #424242;");
    codeEditor->setCompleter(completer);

    return codeEditor;
}

QAbstractItemModel *MainWindow::modelFromFile(QCompleter *c, const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
        return new QStringListModel(c);

#ifndef QT_NO_CURSOR
    //QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif
    QStringList words;

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        if (!line.isEmpty()) {
            words << line.trimmed();
        }
    }

#ifndef QT_NO_CURSOR
    //QApplication::restoreOverrideCursor();
#endif
    return new QStringListModel(words, c);
}

void MainWindow::find() {
    if (ui->tabs->currentIndex() == 0)
        return;
    
    if (!mSearchWidgetAdded) {
        ui->statusBar->addWidget(mSearchWidget);
    }
    mSearchWidget->show();
    mSearchWidget->lineEdit()->setFocus(Qt::MouseFocusReason);
    mSearchWidgetAdded = true;
}

void MainWindow::closeFind() {
    ui->statusBar->removeWidget(mSearchWidget);
}

void MainWindow::save() {
    QString dir = QFileDialog::getSaveFileName(this, "Save...", "", "*.fred", 0, 0);

    if (dir.isEmpty()) {
        return;
    }

    QFile file(dir);

    if (!file.open(QIODevice::WriteOnly)) {
        ui->console->setTextColor(mRed);
        ui->console->append("Error opening file for save.");
        ui->console->setTextColor(mBlack);
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_0);
    out << mThresholdDiablog->rMin << mThresholdDiablog->gMin << mThresholdDiablog->bMin << mThresholdDiablog->rMax
        << mThresholdDiablog->gMax << mThresholdDiablog->bMax << mTimerTime;

    file.flush();
    file.close();
}

void MainWindow::load() {
    QString dir = QFileDialog::getOpenFileName(this, "Open...", "", "*.fred", 0, 0);

    if (dir.isEmpty()) {
        return;
    }

    QFile file(dir);

    if (!file.open(QIODevice::ReadOnly)) {
        ui->console->setTextColor(mRed);
        ui->console->append("Error opening file for reading.");
        ui->console->setTextColor(mBlack);
        return;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_0);
    in >> mThresholdDiablog->rMin >> mThresholdDiablog->gMin >> mThresholdDiablog->bMin >> mThresholdDiablog->rMax
        >> mThresholdDiablog->gMax >> mThresholdDiablog->bMax >> mTimerTime;

    mThresholdDiablog->updateSliders();
    updateThreshold();

    file.close();
}

void MainWindow::processFrameAndUpdateGUI() {
    if (!mCameras.at(0)->isOpen())
        return;
    
    for (int i = 0; i <mCameras.size(); i++) {
        mCameras.at(i)->render();
    }
    
    ui->originalImage->setPixmap(mCameras.at(0)->getPix());
    
    if (mNcams > 1) {
        ui->processedImage->setPixmap(mCameras.at(1)->getPix());
    } else {
        ui->processedImage->setPixmap(mCameras.at(0)->getPix());
    }
    
    //needs updating V
    //mHistogramDialog->updatHistogram(mMatOriginal);
    
    if (mOutlineDialog->isVisible()) {
        doOutline();
        QImage qimgOutline2((uchar*)mMatDetectedEdges.data, mMatDetectedEdges.cols, mMatDetectedEdges.rows, mMatDetectedEdges.step, QImage::Format_Indexed8);
        mOutlineDialog->setLabelPixmap(qimgOutline2);
    }
    
    updateFPS();
}

void MainWindow::updateFPS() {
    mFPS->update();
}

void MainWindow::displayFPS() {
    mFPSLabel->setText(QString::number(mFPS->getFPS()));
}


void MainWindow::refresh(){
    bool r = false;
    
    //pause
    if (mIsRunning) {
        pauseButtonClicked();
        r = true;
    }
    
    debug("closing");
    std::cout << mNcams << "cams" << std::endl;
    for (int i = 0; !mCameras.isEmpty(); i++) {
        std::cout << i << std::endl;
        Camera *c = mCameras.takeAt(0);
        std::cout << "pre close" << std::endl;
        c->close();
        std::cout << "close" << std::endl;
        delete c;
    }
    debug("closed");
    
    mNcams = getCamCount();
    debug(QString::number(mNcams) + " camera(s) detected.");
    std::cout << mNcams << "detected\n";
    createCameras();
    
    //unpause
    if (r) {
        pauseButtonClicked();
    }
}

void MainWindow::doCircles() {
    //0, 120, 0     170, 256, 40 for the green thing
    cv::inRange(mMatOriginal, cv::Scalar(bMin, gMin, rMin), cv::Scalar(bMax, gMax, rMax), mMatProcessed);
    cv::GaussianBlur(mMatProcessed, mMatProcessed, cv::Size(9, 9), 1.5);
    cv::HoughCircles(mMatProcessed, mVecCircles, CV_HOUGH_GRADIENT, 1, mMatProcessed.rows / 4, 100, 50, 10, 400);

    for (mItrCircles = mVecCircles.begin(); mItrCircles != mVecCircles.end(); mItrCircles++) {
        ui->console->setTextColor(mGreen);
        ui->console->append("pos x =" + QString::number((*mItrCircles)[0]).rightJustified(4, ' ') +
                ", y =" + QString::number((*mItrCircles)[1]).rightJustified(4, ' ') +
                ", radius =" + QString::number((*mItrCircles)[2], 'f', 3).rightJustified(7, ' '));

        //ui->console->setTextColor(mBlack);

        cv::circle(mMatOriginal, cv::Point((int)(*mItrCircles)[0], (int)(*mItrCircles)[1]), 3, cv::Scalar(0, 255, 0), CV_FILLED);
        cv::circle(mMatOriginal, cv::Point((int)(*mItrCircles)[0], (int)(*mItrCircles)[1]), (int)(*mItrCircles)[2], cv::Scalar(0, 0, 255), 3);
    }
}

void MainWindow::doOutline() {
    //outline
    mMatOriginal = mCameras.at(0)->get().clone();
    mMatOutline.create(mMatOriginal.size(), mMatOriginal.type());

    cv::cvtColor(mMatOriginal, mMatGray, CV_BGR2GRAY);

    cv::blur(mMatGray, mMatDetectedEdges, cv::Size(mKernelSize, mKernelSize));

    cv::Canny(mMatDetectedEdges, mMatDetectedEdges, mOutlineDialog->getSliderValue(), mOutlineDialog->getSliderValue() * 3, mKernelSize);

    //matOutline = cv::Scalar::all(0);

    //matOriginal.copyTo(matOutline, matDetectedEdges);
    mMatDetectedEdges.copyTo(mMatOutline, mMatDetectedEdges);
}

void MainWindow::updateStatusLabel(const QString &text) {
    mStatusLabel->setText(text);
}

void MainWindow::pauseButtonClicked() {
    if (!ui->actionStart->isEnabled()) {
        mTimer->stop();
        mFPSTimer->stop();

        ui->actionPause->setDisabled(true);
        ui->actionStart->setEnabled(true);

        debug("Pausing");
        mIsRunning = false;
    } else {
        mTimer->start(mTimerTime);
        mFPSTimer->start(1000);

        ui->actionPause->setDisabled(false);
        ui->actionStart->setEnabled(false);

        debug("Resuming");
        mIsRunning = true;
        ui->tabs->setCurrentIndex(0);
    }
}

void MainWindow::thresholdClicked() {
    if (!mThresholdDiablog) {
        mThresholdDiablog = new ColorThresholdDialog(this);
    }

    if (mThresholdDiablog->isHidden() == false) {
        //ui->thresholdDock->setHidden(false);
        mThresholdDiablog->close();

        this->rMin = mThresholdDiablog->rMin;
        this->gMin = mThresholdDiablog->gMin;
        this->bMin = mThresholdDiablog->bMin;
        this->rMax = mThresholdDiablog->rMax;
        this->gMax = mThresholdDiablog->gMax;
        this->bMax = mThresholdDiablog->bMax;
    } else {
        //ui->thresholdDock->setHidden(true);
        mThresholdDiablog->show();
    }
}

void MainWindow::updateThreshold() {
    this->rMin = mThresholdDiablog->rMin;
    this->gMin = mThresholdDiablog->gMin;
    this->bMin = mThresholdDiablog->bMin;
    this->rMax = mThresholdDiablog->rMax;
    this->gMax = mThresholdDiablog->gMax;
    this->bMax = mThresholdDiablog->bMax;

    if (ui->actionThreshold->isChecked()) {
        ui->actionThreshold->setChecked(false);
    }
}

void MainWindow::hideThreshold() {
    mThresholdDiablog->close();
    ui->actionThreshold->setChecked(false);
}

void MainWindow::clearConsoleClicked() {
    ui->console->setText("");
    debug("Clear console");
}

void MainWindow::hideConsole() {
    if (!mSearchWidgetAdded) {
        ui->consoleDock->setHidden(!ui->consoleDock->isHidden());
        //ui->commandsDock->setHidden(!ui->commandsDock->isHidden());
    } else {
        closeFind();
        mSearchWidgetAdded = false;
    }
}

void MainWindow::showToolbarClicked() {
    if(ui->mainToolBar->isHidden()) {
        ui->mainToolBar->show();
    } else {
        ui->mainToolBar->hide();
    }
}

void MainWindow::showHistogramClicked() {
    if (mHistogramDialog->isHidden()) {
        mHistogramDialog->show();
    } else {
        mHistogramDialog->hide();
    }
}

void MainWindow::showOutlineClicked() {
    if (mOutlineDialog->isHidden()) {
        mOutlineDialog->show();
    } else {
        mOutlineDialog->hide();
    }
}

void MainWindow::showSquaresClicked() {
    if (mSquaresDialog->isHidden()) {
        mSquaresDialog->show();
    } else {
        mSquaresDialog->hide();
    }
}

void MainWindow::aboutDialogClicked() {
    AboutDialog a(this);
    a.exec();
}

void MainWindow::newCommandClicked() {
    
}

void MainWindow::addCommand(Command<MainWindow> c) {
    //Q_ASSERT(c);
    
    mCommandList.append(c);
    
    ProjectItem *itm = new ProjectItem();
    itm->setText(0, c.getName());
    itm->setData(0, Qt::UserRole, ORIGINAL);

    addChild(mRoot, itm);
}
