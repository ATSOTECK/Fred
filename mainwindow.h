#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QFile>
#include <QToolButton>
#include <QFileDialog>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc.hpp>

#include "colorthresholddialog.h"
#include "histogramdialog.h"
#include "outlinedialog.h"
#include "squaredialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    int getCamCount();

public slots:
    void processFrameAndUpdateGUI();
    void updateThreshold();
    
private slots:
    void pauseButtonClicked();
    void thresholdClicked();
    void hideThreshold();
    void clearConsoleClicked();
    void showToolbarClicked();
    void showHistogramClicked();
    void showOutlineClicked();
    void showSquaresClicked();

    void save();
    void load();

private:
    void doOutline();

    Ui::MainWindow *ui;

    cv::VideoCapture camera;

    cv::Mat matOriginal;
    cv::Mat matProcessed;
    cv::Mat matOutline;
    cv::Mat matGray;
    cv::Mat matDetectedEdges;

    QImage qimgOriginal;
    QImage qimgProcessed;
    QImage qimgOutline;

    std::vector<cv::Vec3f> vecCircles;
    std::vector<cv::Vec3f>::iterator itrCircles;

    QTimer *timer;

    ColorThresholdDialog *thresholdDiablog;
    HistogramDialog *histogramDialog;
    OutlineDialog *outlineDialog;
    SquareDialog *squaresDialog;

    int rMin, gMin, bMin, rMax, gMax, bMax;
    int timerTime, ncams, kernelSize;

    std::vector<std::vector<cv::Point> > squares;
};

#endif // MAINWINDOW_H
