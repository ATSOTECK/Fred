#ifndef FPS_H
#define FPS_H

#include <QTime>

class FPS {
public:
    FPS();
    
    void update();
    
    int getFPS();
    
private:
    int mOldTime;
    int mLastTime;
    
    int mNumFrames;
    int mFrames;
    
    QTime mTime;
};

#endif // FPS_H
