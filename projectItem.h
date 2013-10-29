#ifndef PROJECTITEM_H
#define PROJECTITEM_H

#include <QString>
#include <QTreeWidgetItem>

class ProjectItem : public QTreeWidgetItem {
public:
    enum Type {
        NONE = 0,
        Command,
        
        CommandRoot,
        INVALID = 1000
    };
    
    ProjectItem(QTreeWidget *parent = 0);
    
    void setType(Type type);
    
    Type getType() {
        return mType;
    }
    
    void setPath(const QString &path);
    
    QString getPath() const {
        return mPath;
    }
    
private:
    Type mType;
    QString mPath;
};

#endif // PROJECTITEM_H
