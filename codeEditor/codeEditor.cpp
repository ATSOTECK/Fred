#include "codeEditor.h"

//#include "preferences.h"

#include <QtGui>
#include <QCompleter>
#include <QKeyEvent>
#include <QAbstractItemView>
#include <QtDebug>
#include <QApplication>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QScrollBar>
#include <QTimer>
#include <QFile>
#include <QMessageBox>

CodeEditor::CodeEditor(QString name, QWidget *parent, Highlighter *h):
    QPlainTextEdit(parent),
    c(0),
    mName(name),
    set(false),
    mIsModified(false)
{
    lineNumberArea = new LineNumberArea(this);
    highlighter = h;
    
    mTimer = new QTimer(this);
    
    //Preferences *prefs = Preferences::instance();
    
    bool b = true;//prefs->showCodeEditorMiniMap();
    
    if (b) {
        Highlighter *miniHighlighter = 0;
        mMiniMap = new MiniMapC(this, miniHighlighter);
        miniHighlighter = new Highlighter(mMiniMap->document());
        connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateMiniMapVisibleArea()));
        connect(mTimer, SIGNAL(timeout()), this, SLOT(updateMiniMapText()));
        connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateMiniMapScrollPos()));
    } else {
        mMiniMap = 0;
    }

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect, int)), this, SLOT(updateLineNumberArea(QRect, int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightSelectedWord()));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(blockOrColumnChanged()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
    highlightSelectedWord();

    this->setCenterOnScroll(true);
    this->setWordWrapMode(QTextOption::NoWrap);

    if (mMiniMap) {
        mMiniMap->adjustToParent();
        updateMiniMapText();
        mTimer->start(5000);
    }
}

CodeEditor::~CodeEditor() {

}

QTextBlock CodeEditor::editorFirstVisibleBlock() {
    return firstVisibleBlock();
}

QPointF CodeEditor::editorContentOffset() {
    return contentOffset();
}

QRectF CodeEditor::editorBlockBoundingGeometry(const QTextBlock &block) const {
    return blockBoundingGeometry(block);
}

int CodeEditor::lineNumberAreaWidth() {
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space + 18;
}

void CodeEditor::updateLineNumberAreaWidth(int /*newBlockCount*/) {
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy) {
    if (dy) {
        lineNumberArea->scroll(0, dy);
    } else {
        lineNumberArea->update(0, rect.y(), rect.width(), rect.height());
    }

    if (rect.contains(viewport()->rect())) {
        updateLineNumberAreaWidth(0);
    }
}

void CodeEditor::resizeEvent(QResizeEvent *e) {
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));

    if (mMiniMap)
        mMiniMap->adjustToParent();
}

void CodeEditor::blockOrColumnChanged() {
    int b = textCursor().blockNumber() + 1;
    int t = blockCount();
    int c = textCursor().columnNumber();
    QString statusInfo = QString("%1/%3: %2").arg(b).arg(c).arg(t);
    emit statusInfoChanged(statusInfo);
}

void CodeEditor::highlightCurrentLine() {
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::darkGray).darker(320);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void CodeEditor::highlightSelectedWord() {
    QString word = textUnderCursor();
    if (word == "")
        return;
    if (word != mSelectedText) {
        mSelectedText = word;
        highlighter->setSelectedWord(word);
        QList<int> lines;
        int pos = 0;
        int wordLength = word.length();
        QTextCursor cursor = document()->find(word, pos, QTextDocument::FindCaseSensitively);
        while (cursor.position() != -1) {
            int t = cursor.blockNumber();
            lines.append(t);
            pos = cursor.position() + wordLength;
            cursor = document()->find(word, pos, QTextDocument::FindCaseSensitively);
        }
        //int size = lines.size();
        //for (int  i = 0; i < size; ++i) {
           // highlighter->rehighlightLines(lines.at(i));
        //}
    }
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event) {
    int pageBottom = this->viewport()->height();
    //QTextBlock currentBlock = this->document()->findBlock(this->textCursor().position());

    QPainter painter(lineNumberArea);
    painter.fillRect(lineNumberArea->rect(), QColor(Qt::darkGray).darker(280));

    QTextBlock block = firstVisibleBlock();
    QPointF viewportOffset = contentOffset();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(QColor(Qt::white).darker(180));
            painter.drawText(-18, top, lineNumberArea->width(), fontMetrics().height(), Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }

    //code folding
    foldArea = 15;
    int xofs = lineNumberArea->width() - foldArea;
    QRect r(xofs, 0, foldArea, lineNumberArea->height());
    painter.fillRect(r, QColor(Qt::lightGray).darker(100));
    if (foldArea != rightArrowIcon.width()) {
        QPolygonF polygon;// = new QPolygonF();

        rightArrowIcon = QPixmap(foldArea, foldArea);
        rightArrowIcon.fill(Qt::transparent);
        downArrowIcon = QPixmap(foldArea, foldArea);
        downArrowIcon.fill(Qt::transparent);

        polygon.append(QPointF(foldArea * 0.4, foldArea * 0.25));
        polygon.append(QPointF(foldArea * 0.4, foldArea * 0.75));
        polygon.append(QPointF(foldArea * 0.8, foldArea * 0.5));
        QPainter iconPainter(&rightArrowIcon);
        iconPainter.setPen(Qt::NoPen);
        iconPainter.setBrush(QColor(Qt::lightGray).darker(150));
        iconPainter.drawPolygon(polygon);

        polygon.clear();
        polygon.append(QPointF(foldArea * 0.25, foldArea * 0.4));
        polygon.append(QPointF(foldArea * 0.75, foldArea * 0.4));
        polygon.append(QPointF(foldArea * 0.5, foldArea * 0.8));
        QPainter iconPainterd(&downArrowIcon);
        iconPainterd.setPen(Qt::NoPen);
        iconPainterd.setBrush(QColor(Qt::lightGray).darker(150));
        iconPainterd.drawPolygon(polygon);
    }

    block = firstVisibleBlock();
    while (block.isValid()) {
        QPointF position = this->blockBoundingGeometry(block).topLeft() + viewportOffset;

        if (position.y() > pageBottom) {
            break;
        }

        //painter.drawPixmap(xofs, round(position.y()), downArrowIcon);
        //painter.drawPixmap(xofs, round(position.y()), rightArrowIcon);

        if (breakPoints.contains(block.blockNumber())) {
            QLinearGradient linearGradient = QLinearGradient(xofs, round(position.y()), xofs + foldArea, round(position.y()) + foldArea);
            linearGradient.setColorAt(0, QColor(255, 11, 11));
            linearGradient.setColorAt(1, QColor(147, 9, 9));
            painter.setRenderHints(painter.Antialiasing, true);
            painter.setPen(Qt::NoPen);
            painter.setBrush(QBrush(linearGradient));
            painter.drawEllipse(xofs + 1, round(position.y()), foldArea - 2, foldArea - 1);
        }

        if (/*mBookmarks->contains(block)*/bookmarks.contains(block.blockNumber())) {
            QLinearGradient linearGradient = QLinearGradient(xofs, round(position.y()), xofs + foldArea, round(position.y()) + foldArea);
            linearGradient.setColorAt(0, QColor(13, 62, 243));
            linearGradient.setColorAt(1, QColor(5, 27, 106));
            painter.setRenderHints(painter.Antialiasing, true);
            painter.setPen(Qt::NoPen);
            painter.setBrush(QBrush(linearGradient));
            painter.drawRoundRect(xofs + 1, round(position.y()), foldArea - 2, foldArea - 1);
        }

        block = block.next();
    }

    painter.end();
    QWidget::paintEvent(event);
}

void CodeEditor::updateMiniMapText() {
    if (!mMiniMap)
        return;
    
    if (set)
        return;
    
    //if (!set)
        //set = true;

    //QTextDocument *bla = document();
    QString bla = toPlainText();
    mMiniMap->setCode(bla);
    updateMiniMapScrollPos();
}

void CodeEditor::updateMiniMapVisibleArea() {
    if (!mMiniMap)
        return;

    mMiniMap->updateVisibleArea();
}

void CodeEditor::updateMiniMapScrollPos() {
    if (!mMiniMap)
        return;

    mMiniMap->verticalScrollBar()->setValue(verticalScrollBar()->value());
}

void LineNumberArea::mousePressEvent(QMouseEvent *e) {
    int xofs = width() - codeEditor->foldArea;
    int fh = codeEditor->fontMetrics().lineSpacing();
    float ys = e->pos().y();
    //int lineNumber = 0;
    if (e->pos().x() > xofs) {
        QTextBlock block = codeEditor->editorFirstVisibleBlock();
        int pageBottom = codeEditor->viewport()->height();
        QPointF viewportOffset = codeEditor->editorContentOffset();

        while (block.isValid()) {
            QPointF position = codeEditor->editorBlockBoundingGeometry(block).topLeft() + viewportOffset;
            if (position.y() > pageBottom) {
                break;
            }

            if (position.y() < ys && (position.y() + fh) > ys && e->button() == Qt::LeftButton) {
                int line = block.blockNumber();

                if (codeEditor->bookmarks.contains(line)) {
                    break;
                }

                if (codeEditor->breakPoints.contains(line)) {
                    codeEditor->breakPoints.remove(codeEditor->breakPoints.indexOf(line));
                } else {
                    codeEditor->breakPoints.append(line);
                }
                update();
                break;
            } else if (position.y() < ys && (position.y() + fh) > ys && e->button() == Qt::RightButton) {
                int line = block.blockNumber();

                if (codeEditor->breakPoints.contains(line)) {
                    break;
                }

                if (/*codeEditor->mBookmarks->contains(block)*/codeEditor->bookmarks.contains(line)) {
                    codeEditor->bookmarks.remove(codeEditor->bookmarks.indexOf(line));
                    //codeEditor->mBookmarks->removeAt(codeEditor->mBookmarks->indexOf(block));
                } else {
                    codeEditor->bookmarks.append(line);
                    //codeEditor->mBookmarks->append(block);
                }
                update();
                break;
            }
            block = block.next();
        }
    }
}

//TODO: get sensativity correct

void LineNumberArea::wheelEvent(QWheelEvent *event) {
    //event->accept();
    int degrees = event->delta() / 8;
    float steps = degrees / 3;
    if (event->orientation() == Qt::Vertical) {
        codeEditor->verticalScrollBar()->setValue(codeEditor->verticalScrollBar()->value() - steps);
    } else if (event->orientation() == Qt::Horizontal) {
        codeEditor->horizontalScrollBar()->setValue(codeEditor->horizontalScrollBar()->value() - event->delta());
    }
}

void CodeEditor::wheelEvent(QWheelEvent *e) {
    QPlainTextEdit::wheelEvent(e);
    /*
    float steps = e->delta() / 8;
    if (e->orientation() == Qt::Vertical) {
        float s = steps;
        if (fabs(steps) > 20) {
            steps = 20;
            if (s < 0) {
                steps *= -1;
            }
        }
        
        if (fabs(steps) > 0 && fabs(steps) < 1) {
            steps = 1;
            if (s < 0) {
                steps *= -1;
            }
        }
        verticalScrollBar()->setValue(verticalScrollBar()->value() - steps);
        if (mMiniMap)
            mMiniMap->verticalScrollBar()->setValue(verticalScrollBar()->value() - steps);
    } else if (e->orientation() == Qt::Horizontal) {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - e->delta());
    }
    */
}

void CodeEditor::setCompleter(QCompleter *completer) {
    if (c) {
        QObject::disconnect(c, 0, this, 0);
    }

    c = completer;

    if (!c)
        return;

    c->setWidget(this);
    c->setCompletionMode(QCompleter::PopupCompletion);
    c->setCaseSensitivity(Qt::CaseInsensitive);
    QObject::connect(c, SIGNAL(activated(QString)), this, SLOT(insertCompletion(QString)));
}

QCompleter *CodeEditor::completer() const {
    return c;
}

void CodeEditor::insertCompletion(const QString &completion) {
    if (c->widget() != this)
        return;

    QTextCursor tc = textCursor();
    int extra = completion.length() - c->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    setTextCursor(tc);
}

QString CodeEditor::textUnderCursor() const {
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}

void CodeEditor::focusInEvent(QFocusEvent *e) {
    if (c)
        c->setWidget(this);

    QPlainTextEdit::focusInEvent(e);
}

void CodeEditor::keyPressEvent(QKeyEvent *e) {
    if (c && c->popup()->isVisible()) {
        switch (e->key()) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
        case Qt::Key_Escape:
        //case Qt::Key_Tab:
        case Qt::Key_Backtab:
            e->ignore();
            return;
        default:
            break;
        }
    }
    
    mSelectedText = textCursor().selectedText();
    
    switch (e->key()) {
    case Qt::Key_Apostrophe:
    case Qt::Key_QuoteDbl:
        //TODO: check settings
        completeQuotes(e->text());
        return;
        break;
    case Qt::Key_BraceLeft:
    case Qt::Key_BracketLeft:
    case Qt::Key_ParenLeft:
        completeBraces(e->text());
        return;
        break;
    case Qt::Key_Tab:
        insertIndentation();
        return;
        break;
    default:
        break;
    }

    bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_E); //command+E or control+E
    if (!c || !isShortcut) {
        QPlainTextEdit::keyPressEvent(e);
    }
    
    if (Qt::ControlModifier && e->key() == Qt::Key_V) {
        updateMiniMapText();
    }
    
    if (e->key() == Qt::Key_Tab && c && c->popup()->isVisible()) {
        c->popup()->hide();
    }

    const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
    if (!c || (ctrlOrShift && e->text().isEmpty())) {
        return;
    }

    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
    bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    QString completionPrefix = textUnderCursor();

    if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 3 || eow.contains(e->text().right(1)))) {
        c->popup()->hide();
        return;
    }

    if (completionPrefix != c->completionPrefix()) {
        c->setCompletionPrefix(completionPrefix);
        c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
    }

    QRect cr = cursorRect();
    cr.setWidth(c->popup()->sizeHintForColumn(0) + c->popup()->verticalScrollBar()->sizeHint().width());
    c->complete(cr);
}

void CodeEditor::mouseReleaseEvent(QMouseEvent *e) {
    QPlainTextEdit::mouseReleaseEvent(e);
    
    if (e->button() == Qt::LeftButton) {
        //highlightSelectedWord();
    }
}

void CodeEditor::completeBraces(QString text) {
    QString t;
    
    if (text == "{") {
        t = "}";
    } else if (text == "[") {
        t = "]";
    } else if (text == "(") {
        t = ")";
    }
    
    if (mSelectedText.length() > 0) {
        textCursor().insertText(text);        
        textCursor().insertText(mSelectedText);
        textCursor().insertText(t);
        moveCursor(QTextCursor::Left);
    } else {
        textCursor().insertText(text);        
        textCursor().insertText(t);
        moveCursor(QTextCursor::Left);
    }
}

void CodeEditor::completeQuotes(QString text) {
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);
    
    if (mSelectedText.length() > 0) {
        textCursor().insertText(text);
        textCursor().insertText(mSelectedText);
        textCursor().insertText(text);
        moveCursor(QTextCursor::Left);
        //textCursor().select(QTextCursor::WordUnderCursor);
    } else {
        textCursor().insertText(text);
        textCursor().insertText(text);
        moveCursor(QTextCursor::Left);
    }
}

void CodeEditor::insertIndentation() {
    if (textCursor().hasSelection()) {
        indentMore();
    }
    for (int i = 0; i < 4; ++i) {
        textCursor().insertText(" ");
    }
}

void CodeEditor::indentMore() {
    QTextCursor cursor = textCursor();
    QTextBlock block = document()->findBlock(cursor.selectionStart());
    QTextBlock end = document()->findBlock(cursor.selectionStart()).next();
    
    cursor.beginEditBlock();
    
    cursor.setPosition(block.position());
    while (block != end) {
        cursor.setPosition(block.position());
        for (int i = 0; i < 4; ++i) {
            textCursor().insertText(" ");
        }
        block= block.next();
    }
    cursor.endEditBlock();
}

QString CodeEditor::reverseSelectTextPortionFromOffset(int begin, int end) {
    QTextCursor cur = textCursor();
    int cur_pos = cur.position();
    cur.setPosition(cur_pos + begin);
    while ((cur.position() == cur_pos) && begin > 0) {
        begin -= 1;
        cur.setPosition(cur_pos + begin);
    }
    cur.setPosition(cur_pos - end, QTextCursor::KeepAnchor);
    QString text = cur.selectedText();
    return text;
}

void CodeEditor::findText() {

}

bool CodeEditor::saveToFile(QString &path) {
    QString fileName = path + '/' + mName;
    if (!fileName.contains(".lua")) {
        fileName += ".lua";
    }
    
    QFile file(fileName);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&file);
        
        out << toPlainText();
        
        file.flush();
        file.close();
        
        return true;
    }
    
    return false;
}

bool CodeEditor::openFile(const QString &path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "Error", file.errorString());
        return false;
    }
    
    QTextStream in(&file);
    setPlainText(in.readAll());
    
    file.close();
    
    if (mMiniMap) {
        updateMiniMapText();
    }
    
    return true;
}



























