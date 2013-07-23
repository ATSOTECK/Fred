#include "newscriptdialog.h"
#include "ui_newscriptdialog.h"

#include "codeEditor.h"
#include "document.h"
#include "documentManager.h"

#include <QTabWidget>
#include <QLabel>
#include <QTextEdit>
#include <QFont>
#include <QStringListModel>
#include <QApplication>

NewScriptDialog::NewScriptDialog(int n, QWidget *parent):
    QDialog(parent),
    ui(new Ui::NewScriptDialog),
    num(n)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->scriptName->setText("UntitledScript" + QString::number(num));

    dm = DocumentManager::instance();
}

NewScriptDialog::~NewScriptDialog() {
    delete ui;
}

CodeEditor *NewScriptDialog::createCodeEditor() {
    if (exec() != QDialog::Accepted)
        return 0;

    CodeEditor *codeEditor = new CodeEditor(ui->scriptName->text(), 0, highlighter);
    codeEditor->setUndoRedoEnabled(true);
    codeEditor->setTabStopWidth(32);
#ifdef Q_WS_MAC
    int size = 12;
    QFont font("Monaco", size);
#endif
#ifdef Q_OS_WIN
    int size = 10;
    QFont font("Consolas", size);
#endif
#ifdef Q_OS_LINUX
    int size = 10;
    QFont font("Inconsolata-g", size);
#endif
    codeEditor->setFont(font);
    codeEditor->setPlainText("--" + ui->scriptName->text() + ".lua\n");
    highlighter = new Highlighter(codeEditor->document());
    completer = new QCompleter();
    completer->setModel(dm->modelFromFile(":/wordlist.txt"));
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setWrapAround(false);
    completer->popup()->setStyleSheet("color: #848484; background-color: #2E2E2E; selection-background-color: #424242;");
    codeEditor->setCompleter(completer);

    return codeEditor;
}
