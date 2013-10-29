#ifndef PROJECTACTION_H
#define PROJECTACTION_H

#include <QAction>

class ProjectAction : public QAction {
public:
    ProjectAction(QString str, QWidget *parent = 0);
    
    void setIndex(int index);
    int getIndex() {
        return mIndex;
    }
    
private:
    int mIndex;
};

#endif // PROJECTACTION_H
