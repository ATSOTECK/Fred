#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QFile>
#include <QToolButton>
#include <QFileDialog>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "colorthresholddialog.h"
#include "histogramdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

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

    void save();
    void load();

private:
    Ui::MainWindow *ui;

    cv::VideoCapture camera;

    cv::Mat matOriginal;
    cv::Mat matProcessed;

    QImage qimgOriginal;
    QImage qimgProcessed;

    std::vector<cv::Vec3f> vecCircles;
    std::vector<cv::Vec3f>::iterator itrCircles;

    QTimer *timer;

    ColorThresholdDialog *thresholdDiablog;
    HistogramDialog *histogramDialog;

    int rMin, gMin, bMin, rMax, gMax, bMax;
    int timerTime;
};

#endif // MAINWINDOW_H
