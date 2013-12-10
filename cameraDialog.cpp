#include "cameraDialog.h"
#include "ui_cameraDialog.h"

CameraDialog::CameraDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CameraDialog)
{
    ui->setupUi(this);
}

CameraDialog::~CameraDialog() {
    
}

void CameraDialog::setLabelPixmap(QImage p) {
    ui->cameraLabel->setPixmap(QPixmap::fromImage(p));
}


