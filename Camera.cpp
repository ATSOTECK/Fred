#include "Camera.h"

#include <QMediaService>
#include <QMediaRecorder>
#include <QCameraViewfinder>

Camera::Camera() :
    imageCapture(0),
    mediaRecorder(0),
    isCapturingImage(false),
    applicationExiting(false)
{
    QByteArray cameraDevice;

    QActionGroup *videoDevicesGroup = new QActionGroup(this);
    videoDevicesGroup->setExclusive(true);
    foreach(const QByteArray &deviceName, QCamera::availableDevices()) {
        QString description = camera->deviceDescription(deviceName);
        QAction *videoDeviceAction = new QAction(description, videoDevicesGroup);
        videoDeviceAction->setCheckable(true);
        videoDeviceAction->setData(QVariant(deviceName));
        if (cameraDevice.isEmpty()) {
            cameraDevice = deviceName;
            videoDeviceAction->setChecked(true);
        }
        //ui->menuDevices->addAction(videoDeviceAction);
    }

    setCamera(cameraDevice);
}
