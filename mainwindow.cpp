#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "codeEditor/codeEditor.h"

#include <QtCore>

#include <QDebug>


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
    kernelSize(3),
    mStatusLabel(new QLabel)
{
    ui->setupUi(this);
    
    qInstallMessageHandler(catchMessage);
    
    setCentralWidget(ui->tabs);

    mBlack = QColor("black");
    mBlue = QColor("blue");
    mRed = QColor("red");
    mGreen = QColor("green");
    
    ui->console->setReadOnly(true);
    
    debug("Starting...");
    QString version = CV_VERSION;
    debug("OpenCV version " + version);

    //ui->actionStart->setDisabled(true);
    ui->actionPause->setDisabled(true);

    ncams = this->getCamCount();
    debug(QString::number(ncams) + " camera(s) detected.");

    QMenu *devicesMenu = new QMenu();
    for (int i = 0; i < ncams; ++i) {
        QAction *testAction = new QAction("Camera: " + QString::number(i), this);
        devicesMenu->addAction(testAction);
        ui->menuDevices->addAction(testAction);
    }
    //QAction *testAction = new QAction("test item", this);
    //QAction *testAction1 = new QAction("another test item", this);
    //devicesMenu->addAction(testAction);
    //devicesMenu->addAction(testAction1);

    QToolButton *devicesButton = new QToolButton();
    devicesButton->setIcon(QIcon(":/Images/webcam.png"));
    devicesButton->setMenu(devicesMenu);
    devicesButton->setPopupMode(QToolButton::InstantPopup);
    devicesButton->setToolTip("Available devices");
    ui->mainToolBar->addWidget(devicesButton);
    
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

    thresholdDiablog = new ColorThresholdDialog(this);
    histogramDialog = new HistogramDialog(this);
    outlineDialog = new OutlineDialog(this);
    squaresDialog = new SquareDialog(this);

    connect(thresholdDiablog, SIGNAL(updateMainWindowTreshold()), this, SLOT(updateThreshold()));
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
    
    mStatusLabel->setText("");
    ui->statusBar->addWidget(mStatusLabel);

    timerTime = 16;

    camera.open(0);

    if (camera.isOpened() == false) {
        ui->console->setTextColor(mRed);
        ui->console->append("Error: camera not accessed successfully.");
        ui->console->setTextColor(mBlack);
        return;
    }
    
    Command<MainWindow> c("Find Circle", *this, &MainWindow::doCircles);
    addCommand(c);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(processFrameAndUpdateGUI()));
    //timer->start(timerTime);
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

QTreeWidgetItem *MainWindow::addRoot(QString name) {
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->commands);
    itm->setText(0, name);

    return itm;
}

void MainWindow::addChild(QTreeWidgetItem *parent, QString name, QIcon &icon) {
    QTreeWidgetItem *itm = new QTreeWidgetItem();
    itm->setText(0, name);
    itm->setIcon(0, icon);
    itm->setData(0, 32, QVariant(name));
    parent->addChild(itm);
}

void MainWindow::addChild(QTreeWidgetItem *parent, QTreeWidgetItem *child) {
    parent->addChild(child);
}

void MainWindow::setUpCommandDock() {

}

void MainWindow::setUpCommands() {
    ui->commands->setColumnCount(1);
    ui->commands->setHeaderLabel("Commands");
    
    root = addRoot("Running Commands");
    root->setData(0, Qt::UserRole, ROOT);
    root->setExpanded(true);

    QTreeWidgetItem *drawOriginalImageItem = new QTreeWidgetItem();
    drawOriginalImageItem->setText(0, "Draw original image");
    drawOriginalImageItem->setData(0, Qt::UserRole, ORIGINAL);

    addChild(root, drawOriginalImageItem);

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
}

int MainWindow::getCamCount() {
    CvCapture *cap;
    int ncams = 0;
    while (true) {
        cap = cvCreateCameraCapture(ncams++);
        if (cap == NULL)
            break;
        cvReleaseCapture(&cap);
    }

    cvReleaseCapture(&cap);
    return (ncams - 1);
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
    codeEditor->appendPlainText("function update()\n    if not paused() then\n        showOriginalImage(0)\n    end\nend");
    highlighter = new Highlighter(codeEditor->document());
    completer = new QCompleter();
    //completer->setModel(dm->modelFromFile(":/wordlist.txt"));
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setWrapAround(false);
    completer->popup()->setStyleSheet("color: #848484; background-color: #2E2E2E; selection-background-color: #424242;");
    codeEditor->setCompleter(completer);

    return codeEditor;
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
    out << thresholdDiablog->rMin << thresholdDiablog->gMin << thresholdDiablog->bMin << thresholdDiablog->rMax
        << thresholdDiablog->gMax << thresholdDiablog->bMax << timerTime;

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
    in >> thresholdDiablog->rMin >> thresholdDiablog->gMin >> thresholdDiablog->bMin >> thresholdDiablog->rMax
        >> thresholdDiablog->gMax >> thresholdDiablog->bMax >> timerTime;

    thresholdDiablog->updateSliders();
    updateThreshold();

    file.close();
}

void MainWindow::processFrameAndUpdateGUI() {
    camera.read(matOriginal);

    if (matOriginal.empty()) {
        ui->console->setTextColor(mRed);
        ui->console->append("Error: camera not working!");
        ui->console->setTextColor(mBlack);
        return;
    }

    //histogram
    histogramDialog->updatHistogram(matOriginal);

    int size = mCommandList.size();
    for (int i = 0; i < size; ++i) {
        Command<MainWindow> c = mCommandList.at(i);
        if (!c.isPaused() && !c.isRunning()) {
            c.update();
        }
    }
    
    //doOutline();

    //squaresDialog->findSquares(matOriginal, squares);
    //squaresDialog->drawSquares(matOriginal, squares);

    //0, 120, 0     170, 256, 40 for the green thing
    /*
    cv::inRange(matOriginal, cv::Scalar(bMin, gMin, rMin), cv::Scalar(bMax, gMax, rMax), matProcessed);
    cv::GaussianBlur(matProcessed, matProcessed, cv::Size(9, 9), 1.5);
    cv::HoughCircles(matProcessed, vecCircles, CV_HOUGH_GRADIENT, 2, matProcessed.rows / 4, 100, 50, 10, 400);

    for (itrCircles = vecCircles.begin(); itrCircles != vecCircles.end(); itrCircles++) {
        ui->console->setTextColor(mGreen);
        ui->console->append("pos x =" + QString::number((*itrCircles)[0]).rightJustified(4, ' ') +
                ", y =" + QString::number((*itrCircles)[1]).rightJustified(4, ' ') +
                ", radius =" + QString::number((*itrCircles)[2], 'f', 3).rightJustified(7, ' '));

        ui->console->setTextColor(mBlack);

        cv::circle(matOriginal, cv::Point((int)(*itrCircles)[0], (int)(*itrCircles)[1]), 3, cv::Scalar(0, 255, 0), CV_FILLED);
        cv::circle(matOriginal, cv::Point((int)(*itrCircles)[0], (int)(*itrCircles)[1]), (int)(*itrCircles)[2], cv::Scalar(0, 0, 255), 3);
    }
    */
    //convert and display
    cv::cvtColor(matOriginal, matOriginal, CV_BGR2RGB);

    QImage qimgOriginal((uchar*)matOriginal.data, matOriginal.cols, matOriginal.rows, matOriginal.step, QImage::Format_RGB888);
    QImage qimgProcessed((uchar*)matProcessed.data, matProcessed.cols, matProcessed.rows, matProcessed.step, QImage::Format_Indexed8);
    //QImage qimgOutline((uchar*)matOutline.data, matOutline.cols, matOutline.rows, matOutline.step, QImage::Format_Indexed8);
    QImage qimgOutline2((uchar*)matDetectedEdges.data, matDetectedEdges.cols, matDetectedEdges.rows, matDetectedEdges.step, QImage::Format_Indexed8);

    outlineDialog->setLabelPixmap(qimgOutline2);

    ui->originalImage->setPixmap(QPixmap::fromImage(qimgOriginal));
    ui->processedImage->setPixmap(QPixmap::fromImage(qimgProcessed));
}

void MainWindow::doCircles() {
    //0, 120, 0     170, 256, 40 for the green thing
    cv::inRange(matOriginal, cv::Scalar(bMin, gMin, rMin), cv::Scalar(bMax, gMax, rMax), matProcessed);
    cv::GaussianBlur(matProcessed, matProcessed, cv::Size(9, 9), 1.5);
    cv::HoughCircles(matProcessed, vecCircles, CV_HOUGH_GRADIENT, 1, matProcessed.rows / 4, 100, 50, 10, 400);

    for (itrCircles = vecCircles.begin(); itrCircles != vecCircles.end(); itrCircles++) {
        ui->console->setTextColor(mGreen);
        ui->console->append("pos x =" + QString::number((*itrCircles)[0]).rightJustified(4, ' ') +
                ", y =" + QString::number((*itrCircles)[1]).rightJustified(4, ' ') +
                ", radius =" + QString::number((*itrCircles)[2], 'f', 3).rightJustified(7, ' '));

        ui->console->setTextColor(mBlack);

        cv::circle(matOriginal, cv::Point((int)(*itrCircles)[0], (int)(*itrCircles)[1]), 3, cv::Scalar(0, 255, 0), CV_FILLED);
        cv::circle(matOriginal, cv::Point((int)(*itrCircles)[0], (int)(*itrCircles)[1]), (int)(*itrCircles)[2], cv::Scalar(0, 0, 255), 3);
    }
}

void MainWindow::doOutline() {
    //outline
    matOutline.create(matOriginal.size(), matOriginal.type());

    cv::cvtColor(matOriginal, matGray, CV_BGR2GRAY);

    cv::blur(matGray, matDetectedEdges, cv::Size(kernelSize, kernelSize));

    cv::Canny(matDetectedEdges, matDetectedEdges, outlineDialog->getSliderValue(), outlineDialog->getSliderValue() * 3, kernelSize);

    //matOutline = cv::Scalar::all(0);

    //matOriginal.copyTo(matOutline, matDetectedEdges);
    matDetectedEdges.copyTo(matOutline, matDetectedEdges);
}

void MainWindow::updateStatusLabel(const QString &text) {
    mStatusLabel->setText(text);
}

void MainWindow::pauseButtonClicked() {
    if (!ui->actionStart->isEnabled()) {
        timer->stop();

        ui->actionPause->setDisabled(true);
        ui->actionStart->setEnabled(true);

        debug("Pausing");
    } else {
        timer->start(timerTime);

        ui->actionPause->setDisabled(false);
        ui->actionStart->setEnabled(false);

        debug("Resuming");
        ui->tabs->setCurrentIndex(0);
    }
}

void MainWindow::thresholdClicked() {
    if (!thresholdDiablog) {
        thresholdDiablog = new ColorThresholdDialog(this);
    }

    if (thresholdDiablog->isHidden() == false) {
        //ui->thresholdDock->setHidden(false);
        thresholdDiablog->close();

        this->rMin = thresholdDiablog->rMin;
        this->gMin = thresholdDiablog->gMin;
        this->bMin = thresholdDiablog->bMin;
        this->rMax = thresholdDiablog->rMax;
        this->gMax = thresholdDiablog->gMax;
        this->bMax = thresholdDiablog->bMax;
    } else {
        //ui->thresholdDock->setHidden(true);
        thresholdDiablog->show();
    }
}

void MainWindow::updateThreshold() {
    this->rMin = thresholdDiablog->rMin;
    this->gMin = thresholdDiablog->gMin;
    this->bMin = thresholdDiablog->bMin;
    this->rMax = thresholdDiablog->rMax;
    this->gMax = thresholdDiablog->gMax;
    this->bMax = thresholdDiablog->bMax;

    if (ui->actionThreshold->isChecked()) {
        ui->actionThreshold->setChecked(false);
    }
}

void MainWindow::hideThreshold() {
    thresholdDiablog->close();
    ui->actionThreshold->setChecked(false);
}

void MainWindow::clearConsoleClicked() {
    debug("Clear console");
}

void MainWindow::hideConsole() {
    ui->consoleDock->setHidden(!ui->consoleDock->isHidden());
    ui->commandsDock->setHidden(!ui->commandsDock->isHidden());
}

void MainWindow::showToolbarClicked() {
    if(ui->mainToolBar->isHidden()) {
        ui->mainToolBar->show();
    } else {
        ui->mainToolBar->hide();
    }
}

void MainWindow::showHistogramClicked() {
    if (histogramDialog->isHidden()) {
        histogramDialog->show();
    } else {
        histogramDialog->hide();
    }
}

void MainWindow::showOutlineClicked() {
    if (outlineDialog->isHidden()) {
        outlineDialog->show();
    } else {
        outlineDialog->hide();
    }
}

void MainWindow::showSquaresClicked() {
    if (squaresDialog->isHidden()) {
        squaresDialog->show();
    } else {
        squaresDialog->hide();
    }
}

void MainWindow::aboutDialogClicked() {
    AboutDialog a(this);
    a.exec();
}

void MainWindow::newCommandClicked() {
    
}

void MainWindow::addCommand(Command<MainWindow> c) {
    Q_ASSERT(c);
    
    mCommandList.append(c);
    
    QTreeWidgetItem *itm = new QTreeWidgetItem();
    itm->setText(0, c.getName());
    itm->setData(0, Qt::UserRole, ORIGINAL);

    addChild(root, itm);
}























