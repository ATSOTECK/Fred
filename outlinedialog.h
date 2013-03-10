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

    int getSliderValue();

    void setLabelPixmap(QImage p);

private slots:
    void updateLabel();
    
private:
    Ui::OutlineDialog *ui;
};

#endif // OUTLINEDIALOG_H
