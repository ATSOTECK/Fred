#include "histogramdialog.h"
#include "ui_histogramdialog.h"

HistogramDialog::HistogramDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HistogramDialog),
    size(256)
{
    ui->setupUi(this);
}

HistogramDialog::~HistogramDialog() {
    delete ui;
}

void HistogramDialog::updatHistogram(Mat src) {

    if (!src.data) {
        return;
    }

    cv::split(src, bgrPlanes);

    //set the range for bgr
    float range[] = {0, 256};
    const float *histRange = {range};

    bool uniform = true;
    bool accumulate = false;

    //compute histograms
    calcHist(&bgrPlanes[0], 1, 0, Mat(), bHist, 1, &size, &histRange, uniform, accumulate);
    calcHist(&bgrPlanes[1], 1, 0, Mat(), gHist, 1, &size, &histRange, uniform, accumulate);
    calcHist(&bgrPlanes[2], 1, 0, Mat(), rHist, 1, &size, &histRange, uniform, accumulate);

    //determine the histograms for bgr
    int histW = 640;
    int histH = 480;
    int binW = cvRound((double) histW / size);

    Mat histImage(histH, histW, CV_8UC3, Scalar(0, 0, 0));

    //normalize the result to [0, histImage.rows]
    normalize(bHist, bHist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
    normalize(gHist, gHist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
    normalize(rHist, rHist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

    for (int i = 1; i < size; i++) {
        line(histImage, cv::Point(binW * (i - 1), histH - cvRound(bHist.at<float>(i - 1))),
             cv::Point(binW * i, histH - cvRound(bHist.at<float>(i))),
             Scalar(255, 0, 0), 2, 8, 0);
        line(histImage, cv::Point(binW * (i - 1), histH - cvRound(gHist.at<float>(i - 1))),
             cv::Point(binW * i, histH - cvRound(gHist.at<float>(i))),
             Scalar(0, 255, 0), 2, 8, 0);
        line(histImage, cv::Point(binW * (i - 1), histH - cvRound(rHist.at<float>(i - 1))),
             cv::Point(binW * i, histH - cvRound(rHist.at<float>(i))),
             Scalar(0, 0, 255), 2, 8, 0);
    }

    cv::cvtColor(histImage, histImage, CV_BGR2RGB);

    QImage image((uchar*)histImage.data, histImage.cols, histImage.rows, histImage.step, QImage::Format_RGB888);

    ui->histImgLbl->setPixmap(QPixmap::fromImage(image));
}
