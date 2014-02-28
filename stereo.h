#ifndef STEREO_H
#define STEREO_H

#include <QObject>
#include <QList>

#include <opencv/cv.h>
#include <opencv/cxmisc.h>
#include <opencv/cvaux.h>
#include <opencv2/highgui/highgui.hpp>

class Stereo : public QObject {
    Q_OBJECT
    
public:
    Stereo(int width, int height);
    ~Stereo();
    
    
    QList<CvPoint3D32f> points[2];
};

#endif // STEREO_H
