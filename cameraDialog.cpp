#include "cameraDialog.h"
#include "ui_cameraDialog.h"

CameraDialog::CameraDialog(Camera *cam, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CameraDialog)
{
    ui->setupUi(this);
    mCam = cam;
}

CameraDialog::~CameraDialog() {
    
}

void CameraDialog::setLabelPixmap(QImage p) {
    ui->cameraLabel->setPixmap(QPixmap::fromImage(p));
}

void CameraDialog::setLabelPixmap() {
    ui->cameraLabel->setPixmap(mCam->getPix());
}
void CameraDialog::closeEvent(){

    QDialog::close();
    delete this;


}
