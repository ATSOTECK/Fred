#include "fps.h"

FPS::FPS() :
    mTime(QTime())
{
    mOldTime = 0;
    mLastTime = 0;
    
    mFrames = 0;
    mNumFrames = 0;
    
    mTime.start();
}

void FPS::update() {
    if (mOldTime + 1000 < mTime.elapsed()) {
        mOldTime = mTime.elapsed();
        
        mNumFrames = mFrames;
        
        mFrames = 0;
    }
    
    mLastTime = mTime.elapsed();
    
    mFrames++;
}

int FPS::getFPS() {
    return mNumFrames;
}
