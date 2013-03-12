#ifndef SQUAREDIALOG_H
#define SQUAREDIALOG_H

#include <QDialog>
#include <math.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc.hpp>

namespace Ui {
class SquareDialog;
}

class SquareDialog : public QDialog
{
    Q_OBJECT

    static double angle(cv::Point p1, cv::Point p2, cv::Point p0);
    
public:
    explicit SquareDialog(QWidget *parent = 0);
    ~SquareDialog();

    void findSquares(const cv::Mat &img, std::vector<std::vector<cv::Point> >&squares);
    void drawSquares(cv::Mat &img, const std::vector<std::vector<cv::Point> >&squares);

private slots:
    void updateLabel();
    
private:
    Ui::SquareDialog *ui;

    int threshold, N;
};

#endif // SQUAREDIALOG_H
