#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    camera(0)
{
    ui->setupUi(this);

    QActionGroup *videoDevicesGroup = new QActionGroup(this);
    videoDevicesGroup->setExclusive(true);
    foreach(const QByteArray &deviceName, QCamera::availableDevices()) {
        QString description = camera->deviceDescription(deviceName);
        QAction *videoDeviceAction = new QAction(description, videoDevicesGroup);
        videoDeviceAction->setCheckable(true);
        videoDeviceAction->setData(QVariant(deviceName));
        if (camera->getCameraDevice().isEmpty()) {
            camera->setCameraDevice(deviceName);
            videoDeviceAction->setChecked(true);
        }
        ui->menuDevices->addAction(videoDeviceAction);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
