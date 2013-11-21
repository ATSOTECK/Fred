#ifndef CAMERA_H
#define CAMERA_H

#include <QWidget>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc.hpp>


class Camera
{

public:
    explicit Camera(int index);
    ~Camera();
    bool open();
    bool close();
    cv::Mat render();
    void setSize(int x, int y);


signals:

public slots:

private:
    cv::VideoCapture mCamera;
    int mIndex;

};

#endif // CAMERA_H
