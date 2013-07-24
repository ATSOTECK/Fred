#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QTextDocumentWriter>
#include <QObject>
#include <QCompleter>
#include <QList>
#include <QWheelEvent>

#include "bookMark.h"
#include "highlighter.h"
#include "miniMapC.h"

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
class QCompleter;
class QTimer;

class LineNumberArea;
class Highlighter;

class CodeEditor : public QPlainTextEdit {
    Q_OBJECT

public:
    CodeEditor(QString name, QWidget *parent = 0, Highlighter *h = 0);
    ~CodeEditor();

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

    //void mousePressEvent(QMouseEvent *e);

    void setCompleter(QCompleter *c);
    QCompleter *completer() const;

    int foldArea;
    QVector<int> breakPoints;
    QVector<int> bookmarks;
    QList<QTextBlock> *mBookmarks;

    //protected in QPlainTextEdit
    QTextBlock editorFirstVisibleBlock();
    QPointF editorContentOffset();
    QRectF editorBlockBoundingGeometry(const QTextBlock &block) const;

    QString getName() const {
        return mName;
    }

    bool saveToFile(QString &path);
    bool openFile(const QString &path);

public slots:
    void findText();
    void updateMiniMapText();
    void updateMiniMapVisibleArea();

protected:
    void resizeEvent(QResizeEvent *event);

    void keyPressEvent(QKeyEvent *e);
    void focusInEvent(QFocusEvent *e);
    
    void mouseReleaseEvent(QMouseEvent *e);

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void highlightSelectedWord();
    void updateLineNumberArea(const QRect & rect, int dy);
    void updateMiniMapScrollPos();
    void insertCompletion(const QString &completion);
    void blockOrColumnChanged();
    
signals:
    void statusInfoChanged(QString info);

private:
    void completeBraces(QString text);
    void completeQuotes(QString text);
    void insertIndentation();
    void indentMore();
    void autoCompleteEnter();
    
    QString getIndentation(const QString &text);
    
    QString reverseSelectTextPortionFromOffset(int begin, int end);
    
    void wheelEvent(QWheelEvent *e);

    QWidget *lineNumberArea;

    QString textUnderCursor() const;
    QString textLeftOfCursor() const;
    QString textRightOfCursor() const;

    QCompleter *c;

    QString mName;

    QPixmap rightArrowIcon;
    QPixmap downArrowIcon;

    Highlighter *highlighter;
    MiniMapC *mMiniMap;

    QTimer *mTimer;
    
    QString mSelectedText;
    
    bool set;
    bool mIsModified;
};

class LineNumberArea : public QWidget {
public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor) {
        codeEditor = editor;
    }

    QSize sizeHint() const {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

    void mousePressEvent(QMouseEvent *e);// {
       // codeEditor->mousePressEvent(e);
    //}

    void wheelEvent(QWheelEvent *event);

private:
    CodeEditor *codeEditor;
};

#endif // CODEEDITOR_H
