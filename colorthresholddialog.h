#ifndef COLORTHRESHOLDDIALOG_H
#define COLORTHRESHOLDDIALOG_H

#include <QDialog>
#include <QBrush>
#include <QPen>

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

    void updateSliders();

signals:
    void updateMainWindowTreshold();

private slots:
    void changeRMinLabel();
    void changeGMinLabel();
    void changeBMinLabel();
    void changeRMaxLabel();
    void changeGMaxLabel();
    void changeBMaxLabel();

protected:
    void paintEvent(QPaintEvent *e);
    
private:
    QBrush brush;

    Ui::ColorThresholdDialog *ui;
};

#endif // COLORTHRESHOLDDIALOG_H
