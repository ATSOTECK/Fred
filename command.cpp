#include "command.h"

template <class T>
void Command<T>::start() {
    mIsRunning = true;
    mIsPaused = false;
}

template <class T>
void Command<T>::stop() {
    mIsRunning = false;
    mIsPaused = false;
}
/*
template <class T>
void Command<T>::update() {
    mTarget.*mFunction();
}
*/
template <class T>
void Command<T>::setRunning(bool running) {
    mIsRunning = running;
}

template <class T>
void Command<T>::setPaused(bool paused) {
    mIsPaused = paused;
}
