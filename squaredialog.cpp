#include "squaredialog.h"
#include "ui_squaredialog.h"

squareDialog::squareDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::squareDialog)
{
    ui->setupUi(this);
}

squareDialog::~squareDialog()
{
    delete ui;
}
