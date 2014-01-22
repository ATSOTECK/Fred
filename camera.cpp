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

void Camera::render() {
    mCamera.read(mMat);
    cv::cvtColor(mMat, mMat, CV_BGR2RGB);
    mPixmap = QPixmap::fromImage( QImage((uchar*)mMat.data, mMat.cols, mMat.rows, mMat.step, QImage::Format_RGB888));
    cv::cvtColor(mMat,mMat,CV_RGB2BGR);
}

cv::Mat Camera::get(){ // carefull this function breaks encapulation
    return mMat;
}

QPixmap Camera::getPix(){
    return mPixmap;
}

void Camera::setSize(int x, int y) {
    mCamera.set(CV_CAP_PROP_FRAME_WIDTH, x);
    mCamera.set(CV_CAP_PROP_FRAME_HEIGHT, y);
}
