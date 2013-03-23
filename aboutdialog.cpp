#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include <QApplication>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}
