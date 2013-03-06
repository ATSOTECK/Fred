#include "colorthresholddialog.h"
#include "ui_colorthresholddialog.h"

ColorThresholdDialog::ColorThresholdDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColorThresholdDialog)
{
    ui->setupUi(this);

    changeRMinLabel();
    changeGMinLabel();
    changeBMinLabel();
    changeRMaxLabel();
    changeGMaxLabel();
    changeBMaxLabel();

    connect(ui->rMin, SIGNAL(valueChanged(int)), this, SLOT(changeRMinLabel()));
    connect(ui->gMin, SIGNAL(valueChanged(int)), this, SLOT(changeGMinLabel()));
    connect(ui->bMin, SIGNAL(valueChanged(int)), this, SLOT(changeBMinLabel()));
    connect(ui->rMax, SIGNAL(valueChanged(int)), this, SLOT(changeRMaxLabel()));
    connect(ui->gMax, SIGNAL(valueChanged(int)), this, SLOT(changeGMaxLabel()));
    connect(ui->bMax, SIGNAL(valueChanged(int)), this, SLOT(changeBMaxLabel()));
}

ColorThresholdDialog::~ColorThresholdDialog()
{
    delete ui;
}

void ColorThresholdDialog::changeRMinLabel() {
    ui->rMinLabel->setText(QString::number(ui->rMin->value()).rightJustified(3, '0'));
}

void ColorThresholdDialog::changeGMinLabel() {
    ui->gMinLabel->setText(QString::number(ui->gMin->value()).rightJustified(3, '0'));
}

void ColorThresholdDialog::changeBMinLabel() {
    ui->bMinLabel->setText(QString::number(ui->bMin->value()).rightJustified(3, '0'));
}

void ColorThresholdDialog::changeRMaxLabel() {
    ui->rMaxLabel->setText(QString::number(ui->rMax->value()).rightJustified(3, '0'));
}

void ColorThresholdDialog::changeGMaxLabel() {
    ui->gMaxLabel->setText(QString::number(ui->gMax->value()).rightJustified(3, '0'));
}

void ColorThresholdDialog::changeBMaxLabel() {
    ui->bMaxLabel->setText(QString::number(ui->bMax->value()).rightJustified(3, '0'));
}
