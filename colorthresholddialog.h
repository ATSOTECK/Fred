#ifndef COLORTHRESHOLDDIALOG_H
#define COLORTHRESHOLDDIALOG_H

#include <QDialog>

namespace Ui {
class ColorThresholdDialog;
}

class ColorThresholdDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ColorThresholdDialog(QWidget *parent = 0);
    ~ColorThresholdDialog();

    int rMin, gMin, bMin, rMax, gMax, bMax;

private slots:
    void changeRMinLabel();
    void changeGMinLabel();
    void changeBMinLabel();
    void changeRMaxLabel();
    void changeGMaxLabel();
    void changeBMaxLabel();
    
private:
    Ui::ColorThresholdDialog *ui;
};

#endif // COLORTHRESHOLDDIALOG_H
