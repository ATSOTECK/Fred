#include "projectAction.h"

ProjectAction::ProjectAction(QString str, QWidget *parent) :
    QAction(str, parent),
    mIndex(0)
{
    
}

void ProjectAction::setIndex(int index) {
    mIndex = index;
}
