#include "camera.h"

#include <iostream>

Camera::Camera(int index, QWidget *parent) :
    QWidget(parent),
    mIndex(index)
{
    open();
}

Camera::~Camera() {
    close();
}

bool Camera::open() {
    if (mCamera.isOpened())
        mCamera.release();

    mCamera.open(mIndex);
    
    std::cout << "here in camera::open\n";
    
    setSize(640, 480);

    if(!mCamera.isOpened()) {
        //qDebug() << "Error: camera " << index << " could not be opened";
        return false;
    }
    
    return true; 
}

bool Camera::isOpen() {
    return mCamera.isOpened();
}

bool Camera::close() {
    if (mCamera.isOpened()) {
        mCamera.release();
        return true;
    }
    
    return false;
}

cv::Mat Camera::render() {
    cv::Mat returnMat;
    mCamera.read(returnMat);
    if (returnMat.empty()) {
        //qDebug() << "Error: camera " << index << "did not recieve an image";
    }
    
    return returnMat;
}

void Camera::setSize(int x, int y) {
    mCamera.set(CV_CAP_PROP_FRAME_WIDTH, x);
    mCamera.set(CV_CAP_PROP_FRAME_HEIGHT, y);
}
