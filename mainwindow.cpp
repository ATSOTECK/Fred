#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QToolButton>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->console->appendPlainText("Starting...");

    ui->actionStart->setDisabled(true);

    QMenu *devicesMenu = new QMenu();
    QAction *testAction = new QAction("test item", this);
    QAction *testAction1 = new QAction("another test item", this);
    devicesMenu->addAction(testAction);
    devicesMenu->addAction(testAction1);

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
    connect(ui->actionClear_console, SIGNAL(triggered()), this, SLOT(clearConsoleClicked()));

    thresholdDiablog = new ColorThresholdDialog(this);

    connect(thresholdDiablog, SIGNAL(accepted()), this, SLOT(hideThreshold()));

    ui->thresholdDock->close();
    //ui->originalImage->close();
    ui->pauseButton->close();

    ui->originalImage->setScaledContents(true);
    ui->processedImage->setScaledContents(true);

    camera.open(0);

    if (camera.isOpened() == false) {
        ui->console->appendPlainText("Error: camera not accessed successfully.");
        return;
    }

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(processFrameAndUpdateGUI()));
    timer->start(20);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::processFrameAndUpdateGUI() {
    camera.read(matOriginal);

    if (matOriginal.empty()) {
        return;
    }

    cv::inRange(matOriginal, cv::Scalar(0, 120, 0), cv::Scalar(170, 256, 40), matProcessed);
    cv::GaussianBlur(matProcessed, matProcessed, cv::Size(9, 9), 1.5);
    cv::HoughCircles(matProcessed, vecCircles, CV_HOUGH_GRADIENT, 2, matProcessed.rows / 4, 100, 50, 10 , 400);

    for (itrCircles = vecCircles.begin(); itrCircles != vecCircles.end(); itrCircles++) {
        ui->console->appendPlainText("ball position x =" + QString::number((*itrCircles)[0]).rightJustified(4, ' ') +
                ", y =" + QString::number((*itrCircles)[1]).rightJustified(4, ' ') +
                ", radius =" + QString::number((*itrCircles)[2], 'f', 3).rightJustified(7, ' '));

        cv::circle(matOriginal, cv::Point((int)(*itrCircles)[0], (int)(*itrCircles)[1]), 3, cv::Scalar(0, 255, 0), CV_FILLED);
        cv::circle(matOriginal, cv::Point((int)(*itrCircles)[0], (int)(*itrCircles)[1]), (int)(*itrCircles)[2], cv::Scalar(0, 0, 255), 3);
    }

    cv::cvtColor(matOriginal, matOriginal, CV_BGR2RGB);

    QImage qimgOriginal((uchar*)matOriginal.data, matOriginal.cols, matOriginal.rows, matOriginal.step, QImage::Format_RGB888);
    QImage qimgProcessed((uchar*)matProcessed.data, matProcessed.cols, matProcessed.rows, matProcessed.step, QImage::Format_Indexed8);

    ui->originalImage->setPixmap(QPixmap::fromImage(qimgOriginal));
    ui->processedImage->setPixmap(QPixmap::fromImage(qimgProcessed));
}

void MainWindow::pauseButtonClicked() {
    if (ui->pauseButton->text() == "Pause") {
        timer->stop();

        ui->pauseButton->setText("Resume");

        ui->actionPause->setDisabled(true);
        ui->actionStart->setEnabled(true);

        ui->console->appendPlainText("Pausing");
    } else {
        timer->start(20);

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
    } else {
        //ui->thresholdDock->setHidden(true);
        thresholdDiablog->show();
    }
}

void MainWindow::hideThreshold() {
    thresholdDiablog->close();
    ui->actionThreshold->setChecked(false);
}

void MainWindow::clearConsoleClicked() {
    ui->console->setPlainText("Clear console");
}
