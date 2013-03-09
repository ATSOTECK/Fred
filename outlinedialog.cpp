#include "outlinedialog.h"
#include "ui_outlinedialog.h"

OutlineDialog::OutlineDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OutlineDialog)
{
    ui->setupUi(this);
}

OutlineDialog::~OutlineDialog()
{
    delete ui;
}
