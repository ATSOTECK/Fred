#ifndef CAMERADIALOG_H
#define CAMERADIALOG_H

#include <QDialog>
#include "camera.h"

namespace Ui {
class CameraDialog;
}

class CameraDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit CameraDialog(Camera * cam, QWidget *parent = 0);
    ~CameraDialog();
    
    void setLabelPixmap(QImage p);
    void setLabelPixmap();
    void closeEvent();
    Camera * mCam;

private:
    Ui::CameraDialog *ui;
};

#endif // CAMERADIALOG_H
