#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->console->appendPlainText("Starting...");

    //ui->actionStart->setDisabled(true);
    ui->actionPause->setDisabled(true);

    ncams = this->getCamCount();

    QMenu *devicesMenu = new QMenu();
    for (int i = 0; i <= ncams; i++) {
        QAction *testAction = new QAction("Camera: " + QString::number(i), this);
        devicesMenu->addAction(testAction);
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

    connect(ui->pauseButton, SIGNAL(clicked()), this, SLOT(pauseButtonClicked()));
    connect(ui->actionPause, SIGNAL(triggered()), this, SLOT(pauseButtonClicked()));
    connect(ui->actionStart, SIGNAL(triggered()), this, SLOT(pauseButtonClicked()));

    connect(ui->actionThreshold, SIGNAL(triggered()), this, SLOT(thresholdClicked()));
    connect(ui->actionHistogram, SIGNAL(triggered()), this, SLOT(showHistogramClicked()));
    connect(ui->actionClear_console, SIGNAL(triggered()), this, SLOT(clearConsoleClicked()));

    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(load()));

    thresholdDiablog = new ColorThresholdDialog(this);
    histogramDialog = new HistogramDialog(this);

    connect(thresholdDiablog, SIGNAL(updateMainWindowTreshold()), this, SLOT(updateThreshold()));
    connect(ui->actionToolbar, SIGNAL(triggered()), this, SLOT(showToolbarClicked()));
    ui->actionToolbar->setChecked(true);

    ui->thresholdDock->close();
    //ui->originalImage->close();
    ui->pauseButton->close();

    ui->originalImage->setScaledContents(true);
    ui->processedImage->setScaledContents(true);

    rMin = gMin = bMin = 0;
    rMax = gMax = bMax = 255;

    timerTime = 16;

    camera.open(0);

    if (camera.isOpened() == false) {
        ui->console->appendPlainText("Error: camera not accessed successfully.");
        return;
    }

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(processFrameAndUpdateGUI()));
    //timer->start(timerTime);
}

MainWindow::~MainWindow() {
    delete ui;
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

void MainWindow::save() {
    QString dir = QFileDialog::getSaveFileName(this, "Save...", "", "*.fred", 0, 0);

    if (dir.isEmpty()) {
        return;
    }

    QFile file(dir);

    if (!file.open(QIODevice::WriteOnly)) {
        ui->console->appendPlainText("Error opening file for save.");
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
        ui->console->appendPlainText("Error opening file for reading.");
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
        return;
    }

    //0, 120, 0     170, 256, 40 for the green thing
    cv::inRange(matOriginal, cv::Scalar(bMin, gMin, rMin), cv::Scalar(bMax, gMax, rMax), matProcessed);
    cv::GaussianBlur(matProcessed, matProcessed, cv::Size(9, 9), 1.5);
    cv::HoughCircles(matProcessed, vecCircles, CV_HOUGH_GRADIENT, 2, matProcessed.rows / 4, 100, 50, 10 , 400);

    for (itrCircles = vecCircles.begin(); itrCircles != vecCircles.end(); itrCircles++) {
        ui->console->appendPlainText("pos x =" + QString::number((*itrCircles)[0]).rightJustified(4, ' ') +
                ", y =" + QString::number((*itrCircles)[1]).rightJustified(4, ' ') +
                ", radius =" + QString::number((*itrCircles)[2], 'f', 3).rightJustified(7, ' '));

        cv::circle(matOriginal, cv::Point((int)(*itrCircles)[0], (int)(*itrCircles)[1]), 3, cv::Scalar(0, 255, 0), CV_FILLED);
        cv::circle(matOriginal, cv::Point((int)(*itrCircles)[0], (int)(*itrCircles)[1]), (int)(*itrCircles)[2], cv::Scalar(0, 0, 255), 3);
    }

    histogramDialog->updatHistogram(matOriginal);

    cv::cvtColor(matOriginal, matOriginal, CV_BGR2RGB);

    QImage qimgOriginal((uchar*)matOriginal.data, matOriginal.cols, matOriginal.rows, matOriginal.step, QImage::Format_RGB888);
    QImage qimgProcessed((uchar*)matProcessed.data, matProcessed.cols, matProcessed.rows, matProcessed.step, QImage::Format_Indexed8);

    ui->originalImage->setPixmap(QPixmap::fromImage(qimgOriginal));
    ui->processedImage->setPixmap(QPixmap::fromImage(qimgProcessed));
}

void MainWindow::pauseButtonClicked() {
    if (!ui->actionStart->isEnabled()) {
        timer->stop();

        ui->pauseButton->setText("Resume");

        ui->actionPause->setDisabled(true);
        ui->actionStart->setEnabled(true);

        ui->console->appendPlainText("Pausing");
    } else {
        timer->start(timerTime);

        ui->pauseButton->setText("Pause");

        ui->actionPause->setDisabled(false);
        ui->actionStart->setEnabled(false);

        ui->console->appendPlainText("Resuming");
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
    ui->console->setPlainText("Clear console");
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
