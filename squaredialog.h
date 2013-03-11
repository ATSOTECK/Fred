#ifndef SQUAREDIALOG_H
#define SQUAREDIALOG_H

#include <QDialog>

namespace Ui {
class squareDialog;
}

class squareDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit squareDialog(QWidget *parent = 0);
    ~squareDialog();
    
private:
    Ui::squareDialog *ui;
};

#endif // SQUAREDIALOG_H
