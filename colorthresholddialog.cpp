#include "colorthresholddialog.h"
#include "ui_colorthresholddialog.h"

#include <QPainter>

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

    connect(ui->buttonBox, SIGNAL(accepted()), this, SIGNAL(updateMainWindowTreshold()));
}

ColorThresholdDialog::~ColorThresholdDialog() {
    delete ui;
}

void ColorThresholdDialog::updateSliders() {
    ui->rMin->setValue(rMin);
    ui->gMin->setValue(gMin);
    ui->bMin->setValue(bMin);
    ui->rMax->setValue(rMax);
    ui->gMax->setValue(gMax);
    ui->bMax->setValue(bMax);
}

void ColorThresholdDialog::changeRMinLabel() {
    ui->rMinLabel->setText(QString::number(ui->rMin->value()).rightJustified(3, '0'));
    rMin = ui->rMin->value();
    repaint();
}

void ColorThresholdDialog::changeGMinLabel() {
    ui->gMinLabel->setText(QString::number(ui->gMin->value()).rightJustified(3, '0'));
    gMin = ui->gMin->value();
    repaint();
}

void ColorThresholdDialog::changeBMinLabel() {
    ui->bMinLabel->setText(QString::number(ui->bMin->value()).rightJustified(3, '0'));
    bMin = ui->bMin->value();
    repaint();
}

void ColorThresholdDialog::changeRMaxLabel() {
    ui->rMaxLabel->setText(QString::number(ui->rMax->value()).rightJustified(3, '0'));
    rMax = ui->rMax->value();
    repaint();
}

void ColorThresholdDialog::changeGMaxLabel() {
    ui->gMaxLabel->setText(QString::number(ui->gMax->value()).rightJustified(3, '0'));
    gMax = ui->gMax->value();
    repaint();
}

void ColorThresholdDialog::changeBMaxLabel() {
    ui->bMaxLabel->setText(QString::number(ui->bMax->value()).rightJustified(3, '0'));
    bMax = ui->bMax->value();
    repaint();
}

void ColorThresholdDialog::paintEvent(QPaintEvent *) {
    QRect rect(390, 30, 50, 50);
    QRect rect2(390, 160, 50, 50);

    QColor color(rMin, gMin, bMin);
    QColor color2(rMax, gMax, bMax);

    QPainter painter(this);
    painter.setBrush(QBrush(color, Qt::SolidPattern));
    painter.drawRect(rect);
    painter.setBrush(QBrush(color2, Qt::SolidPattern));
    painter.drawRect(rect2);
}
