#include "outlinedialog.h"
#include "ui_outlinedialog.h"

OutlineDialog::OutlineDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OutlineDialog)
{
    ui->setupUi(this);

    ui->outlineLbl->setScaledContents(true);

    connect(ui->thresholdSlider, SIGNAL(valueChanged(int)), this, SLOT(updateLabel()));
    updateLabel();
}

OutlineDialog::~OutlineDialog() {
    delete ui;
}

int OutlineDialog::getSliderValue() {
    return ui->thresholdSlider->value();
}

void OutlineDialog::setLabelPixmap(QImage p) {
    ui->outlineLbl->setPixmap(QPixmap::fromImage(p));
}

void OutlineDialog::updateLabel() {
    ui->sliderValueLbl->setText(QString::number(ui->thresholdSlider->value()).rightJustified(3, '0'));
}
