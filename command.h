#ifndef COMMAND_H
#define COMMAND_H

#include <QString>

template <class T>

class Command  {
public:
    typedef void (T::*func)(void);
    
    Command(QString name, T& target, func function) :
        mName(name),
        mTarget(target),
        mFunction(function),
        mIsRunning(false),
        mIsPaused(false)
    {
        
    }
    //Command();
    //Command(const Command&);
    
    QString getName() const {
        return mName;
    }
    
    void start();
    void stop();
    void update() {
        (mTarget.*mFunction)();
    }
    
    void setRunning(bool running);
    bool isRunning() {
        return mIsRunning;
    }
    
    void setPaused(bool paused);
    bool isPaused() {
        return mIsPaused;
    }
    
private:
    QString mName;
    
    T& mTarget;
    func mFunction;
    
    bool mIsRunning;
    bool mIsPaused;
};

#endif // COMMAND_H
