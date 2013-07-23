#include "squaredialog.h"
#include "ui_squaredialog.h"

SquareDialog::SquareDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SquareDialog),
    threshold(50),
    N(11)
{
    ui->setupUi(this);

    ui->squareLbl->setScaledContents(true);

    connect(ui->thresholdSlider, SIGNAL(valueChanged(int)), this, SLOT(updateLabel()));
    updateLabel();
}

SquareDialog::~SquareDialog() {
    delete ui;
}

double SquareDialog::angle(cv::Point p1, cv::Point p2, cv::Point p0) {
    double dx1 = p1.x - p0.x;
    double dy1 = p1.y - p0.y;
    double dx2 = p2.x - p0.x;
    double dy2 = p2.y - p0.y;
    return (dx1 * dx2 + dy1 * dy2) / sqrt((dx1*dx1 + dy1*dy1) * (dx2*dx2 + dy2*dy2) + 1e-10);
}

void SquareDialog::findSquares(const cv::Mat &img, std::vector<std::vector<cv::Point> > &squares) {
    threshold = ui->thresholdSlider->value();
    squares.clear();

    cv::Mat pyr, timg, gray0(img.size(), CV_8U), gray;

    //downscale and upscale the image to filter out noise
    cv::pyrDown(img, pyr, cv::Size(img.cols / 2, img.rows / 2));
    cv::pyrUp(pyr, timg, img.size());
    std::vector<std::vector<cv::Point> > contours;

    //find squares in every color plane of the image
    for (int c = 0; c < 3; c++) {
        //int ch[] = {c, 0};
        //cv::mixChannels(&timg, 1, &gray0, 1, ch, 1);

        //try several threshold levels
        for (int l = 0; l < N; l++) {
            if (l == 0) {
                //apply canny. get upper threshold from slider, set lower to 0 (which forces edges merging)
                cv::Canny(gray0, gray, 0, threshold, 5);
                //dialate canny output to remove any potential holes between edge segments
                cv::dilate(gray, gray, cv::Mat(), cv::Point(-1, -1));
            } else {
                //apply threshold if l != 0
                gray = gray0 >= (l + 1) * 255 / N;
            }

            //find contours and store them in a list
            cv::findContours(gray, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

            std::vector<cv::Point> approx;

            //test each contour
            for (size_t i = 0; i < contours.size(); i++) {
                cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true) * 0.02, true);

                if (approx.size() == 4
                    && fabs(cv::contourArea(cv::Mat(approx))) > 1000
                    && cv::isContourConvex(cv::Mat(approx)))
                {
                    double maxCosine = 0;

                    for (int j = 0; j < 5; j++) {
                        double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
                        maxCosine = MAX(maxCosine, cosine);
                    }

                    if (maxCosine < 0.3) {
                        squares.push_back(approx);
                    }
                }
            }
        }
    }
}

void SquareDialog::drawSquares(cv::Mat &img, const std::vector<std::vector<cv::Point> > &squares) {
    for (size_t i = 0; i < squares.size(); i++) {
        const cv::Point *p = &squares[i][0];
        int n = (int)squares[i].size();
        cv::polylines(img, &p, &n, 1, true, cv::Scalar(0, 255, 0), 3, CV_AA);
    }

    //cv::cvtColor(img, img, CV_BGR2RGB);

    QImage qimg((uchar*)img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);

    ui->squareLbl->setPixmap(QPixmap::fromImage(qimg));
}

void SquareDialog::updateLabel() {
    ui->label->setText(QString::number(ui->thresholdSlider->value()).rightJustified(3, '0'));
}
