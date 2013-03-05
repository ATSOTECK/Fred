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
    setCamera(cameraDevice);
}
