#include "projectItem.h"

ProjectItem::ProjectItem(QTreeWidget *parent) :
    QTreeWidgetItem(parent) {
    
}

void ProjectItem::setType(Type type) {
    mType = type;
}

void ProjectItem::setPath(const QString &path) {
    mPath = path;
}
