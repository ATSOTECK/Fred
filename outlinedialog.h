#ifndef OUTLINEDIALOG_H
#define OUTLINEDIALOG_H

#include <QDialog>

namespace Ui {
class OutlineDialog;
}

class OutlineDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit OutlineDialog(QWidget *parent = 0);
    ~OutlineDialog();
    
private:
    Ui::OutlineDialog *ui;
};

#endif // OUTLINEDIALOG_H
