#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include "ui_aboutdialog.h"

class AboutDialog : public QDialog, private Ui_aboutDialog {
    Q_OBJECT
public:
    AboutDialog(QWidget *parent = 0);
};

#endif // ABOUTDIALOG_H
