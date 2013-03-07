#ifndef HISTOGRAMDIALOG_H
#define HISTOGRAMDIALOG_H

#include <QDialog>
#include <QImage>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

namespace Ui {
class HistogramDialog;
}

class HistogramDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit HistogramDialog(QWidget *parent = 0);
    ~HistogramDialog();

    void updatHistogram(Mat src);
    
private:
    Ui::HistogramDialog *ui;

    Mat src, dst;

    vector<Mat> bgrPlanes;

    int size;

    Mat bHist, gHist, rHist;
};

#endif // HISTOGRAMDIALOG_H
