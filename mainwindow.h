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
#include "projectItem.h"
#include "projectAction.h"
#include "camera.h"

namespace Ui {
class MainWindow;
}

class CodeEditor;
class SearchWidget;

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
    void createCameras();
    
    void debug(const QString &msg);
    void warn(const QString &msg);
    void crit(const QString &msg);
    void fail(const QString &msg);
    
    void setCamera(int c, int w, int h);

public slots:
    void processFrameAndUpdateGUI();
    void updateThreshold();
    
    CodeEditor *addCodeEditor();
    
    QAbstractItemModel *modelFromFile(QCompleter *c, const QString &fileName);
    
    void updateStatusLabel(const QString &text);
    
    void find();
    void closeFind();
    
    void getCam(QAction *c);
    void addCameraDialog(QAction *c);
    
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
    void refresh();
    void save();
    void load();

    void getContextMenu(const QPoint &point);
    
    void hideConsole();
    
    void getCamOneContextMenu(const QPoint &point);

private:
    ProjectItem *addRoot(QString name, ProjectItem::Type type);
    void addChild(ProjectItem *parent, ProjectItem::Type type, QString path, QString name, QIcon &icon);
    void addChild(ProjectItem *parent, ProjectItem *child);

    void setUpCommandDock();
    void setUpCommands();
    void setUpActions();

    void doCircles();
    void doOutline();

    Ui::MainWindow *ui;

    cv::Mat mMatOriginal;
    cv::Mat mMatOriginal2;
    cv::Mat mMatProcessed;
    cv::Mat mMatOutline;
    cv::Mat mMatGray;
    cv::Mat mMatDetectedEdges;

    QImage mQimgOriginal;
    QImage mQimgProcessed;
    QImage mQimgOutline;

    std::vector<cv::Vec3f> mVecCircles;
    std::vector<cv::Vec3f>::iterator mItrCircles;

    QTimer *mTimer;

    ColorThresholdDialog *mThresholdDiablog;
    HistogramDialog *mHistogramDialog;
    OutlineDialog *mOutlineDialog;
    SquareDialog *mSquaresDialog;

    int rMin, gMin, bMin, rMax, gMax, bMax;
    int mTimerTime, mNcams, mKernelSize;

    std::vector<std::vector<cv::Point> > mSquares;
    
    void addCommand(Command<MainWindow> c);
    QList<Command<MainWindow> > mCommandList;

    QList<Camera*> mCameras;

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
    
    ProjectItem *mRoot;
    CodeEditor *mMainCodeEditor;
    
    QLabel *mStatusLabel;
    
    SearchWidget *mSearchWidget;
    bool mSearchWidgetAdded;
    
    QMenu *mCameraMenu;
    bool mIsRunning;
    bool mPoped;
};

void setMainWindow(MainWindow *mw);
void catchMessage(QtMsgType type, const QMessageLogContext &, const QString &msg);

#endif // MAINWINDOW_H
