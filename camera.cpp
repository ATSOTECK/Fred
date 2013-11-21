#include "camera.h"

Camera::Camera(int index):
    mIndex(index)
{


}

Camera::~Camera(){
    close();
}

bool Camera::open(){
    if (mCamera.isOpened())
        mCamera.release();

    mCamera.open(mIndex);
    setSize(640,480);

    if(!mCamera.isOpened()){
     //   qDebug() << "Error: camera " << index << " could not be opened";
    return false;
    }
    return true;
}

bool Camera::close(){
    if (mCamera.isOpened())
        mCamera.release();
    return true;

}

cv::Mat Camera::render(){
    cv::Mat returnMat;
    mCamera >> returnMat;
    if (returnMat.empty()){
     //   qDebug() << "Error: camera " << index << "did not recieve an image";
    }
    return returnMat;

}

void Camera::setSize(int x, int y){
    mCamera.set(CV_CAP_PROP_FRAME_WIDTH, x);
    mCamera.set(CV_CAP_PROP_FRAME_HEIGHT, y);
}
