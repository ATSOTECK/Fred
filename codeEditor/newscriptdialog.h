#ifndef NEWSCRIPTDIALOG_H
#define NEWSCRIPTDIALOG_H

#include "highlighter.h"
#include "documentManager.h"

#include <QDialog>
#include <QtCore>

namespace Ui {
class NewScriptDialog;
}

class CodeEditor;
class QCompleter;
class QAbstractItemModel;

class NewScriptDialog : public QDialog {
    Q_OBJECT

public:
    NewScriptDialog(int n, QWidget *parent = 0);
    ~NewScriptDialog();

    CodeEditor *createCodeEditor();

private:
    Ui::NewScriptDialog *ui;

    Highlighter *highlighter;
    QCompleter *completer;

    DocumentManager *dm;

    int num;
};

#endif // NEWSCRIPTDIALOG_H
