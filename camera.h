#ifndef CAMERA_H
#define CAMERA_H

#include <QWidget>
#include <QDebug>
#include <QPixmap>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc.hpp>


class Camera : public QWidget {
    Q_OBJECT
    
public:
    explicit Camera(int index, QWidget *parent = 0);
    Camera(){} // dont use plz
    ~Camera();
    
    bool open();
    bool isOpen();
    bool close();
    void render();
    void setSize(int x, int y);
    cv::Mat get();
    QPixmap getPix();


private:
    cv::VideoCapture mCamera;
    int mIndex;
    cv::Mat returnMat;
    QPixmap mPmap;
};

#endif // CAMERA_H
