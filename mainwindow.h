#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QFile>
#include <QToolButton>
#include <QFileDialog>
#include <QTreeWidget>
#include <QList>
#include <QLabel>
#include <QCompleter>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc.hpp>

#include "aboutdialog.h"
#include "colorthresholddialog.h"
#include "command.h"
#include "histogramdialog.h"
#include "outlinedialog.h"
#include "squaredialog.h"

namespace Ui {
class MainWindow;
}

class CodeEditor;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    enum Commands {
        ROOT = 1,
        ORIGINAL
    };

    int getCamCount();
    
    void debug(const QString &msg);
    void warn(const QString &msg);
    void crit(const QString &msg);
    void fail(const QString &msg);

public slots:
    void processFrameAndUpdateGUI();
    void updateThreshold();
    
    CodeEditor *addCodeEditor();
    
    QAbstractItemModel *modelFromFile(QCompleter *c, const QString &fileName);
    
    void updateStatusLabel(const QString &text);
    
private slots:
    void pauseButtonClicked();
    void thresholdClicked();
    void hideThreshold();
    void clearConsoleClicked();
    void showToolbarClicked();
    void showHistogramClicked();
    void showOutlineClicked();
    void showSquaresClicked();
    void aboutDialogClicked();
    void newCommandClicked();

    void save();
    void load();

    void getContextMenu(const QPoint &point);
    
    void hideConsole();

private:
    QTreeWidgetItem *addRoot(QString name);
    void addChild(QTreeWidgetItem *parent, QString name, QIcon &icon);
    void addChild(QTreeWidgetItem *parent, QTreeWidgetItem *child);

    void setUpCommandDock();
    void setUpCommands();
    void setUpActions();

    void doCircles();
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
    
    void addCommand(Command<MainWindow> c);
    QList<Command<MainWindow> > mCommandList;

    QAction *mPauseAllCommandsAction;
    QAction *mResumeAllCommandsAction;
    QAction *mPauseSelectedCommandAction;
    QAction *mResumeSelectedCommandAction;
    QAction *mDeleteSelectedCommandAction;
    
    QAction *mHideConsoleAction;

    QColor mBlack;
    QColor mBlue;
    QColor mRed;
    QColor mGreen;
    
    QTreeWidgetItem *root;
    CodeEditor *mMainCodeEditor;
    
    QLabel *mStatusLabel;
};

void setMainWindow(MainWindow *mw);
void catchMessage(QtMsgType type, const QMessageLogContext &, const QString &msg);

#endif // MAINWINDOW_H
