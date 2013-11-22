#ifndef CAMERA_H
#define CAMERA_H

#include <QWidget>
#include <QDebug>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc.hpp>


class Camera : public QWidget {
    Q_OBJECT
    
public:
    explicit Camera(int index, QWidget *parent = 0);
    ~Camera();
    
    bool open();
    bool close();
    cv::Mat render();
    void setSize(int x, int y);

private:
    cv::VideoCapture mCamera;
    int mIndex;
};

#endif // CAMERA_H
