#ifndef CAMERADIALOG_H
#define CAMERADIALOG_H

#include <QDialog>

namespace Ui {
class CameraDialog;
}

class CameraDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit CameraDialog(QWidget *parent = 0);
    ~CameraDialog();
    
    void setLabelPixmap(QImage p);
    
private:
    Ui::CameraDialog *ui;
};

#endif // CAMERADIALOG_H
