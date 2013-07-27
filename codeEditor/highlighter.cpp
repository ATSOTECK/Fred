#include <QtGui>

#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent) :
    QSyntaxHighlighter(parent)
{
    mParent = parent;

    HighlightingRule rule;

    keywordFormat.setForeground(QColor(249, 38, 114));
    //keywordFormat.setFontWeight(QFont::setItalic(true));
    QStringList keywordPatterns;
    keywordPatterns << "\\band\\b" << "\\bbreak\\b" << "\\bdo\\b" << "\\belse\\b" << "\\belseif\\b"
                    << "\\bend\\b" << "\\bfor\\b" << "\\bfunction\\b" << "\\bif\\b"
                    << "\\bin\\b" << "\\blocal\\b" << "\\bnot\\b" << "\\bor\\b"
                    << "\\brepeat\\b" << "\\breturn\\b" << "\\bthen\\b" << "\\buntil\\b"
                    << "\\bwhile\\b" << "\\b=\\b" << "\\b\\+\\b" << "\\b/\\b" << "\\b-\\b" << "\\b\\*\\b"
                    << "\\B=\\B" << "\\B\\+\\B" << "\\B/\\B" << "\\B-\\B" << "\\B\\*\\B" << "\\b\\.\\.\\b" << "\\B\\.\\.\\B"
                    << "\\b<=\\b"<< "\\B<=\\B" << "\\b>=\\b"<< "\\B>=\\B" << "\\b<\\b"<< "\\B<\\B" << "\\b>\\b"<< "\\B>\\B"
                    << "\\#";
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }
    
    functionFormat.setForeground(QColor(160, 226, 45));
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);
    
    //classFormat.setForeground(QColor(0, 160, 190));//new 2ECCFA
    classFormat.setForeground(QColor(0x2E, 0xCC, 0xFA));
    QStringList classPatterns;
    classPatterns << "\\baurora\\b" << "\\bgraphics\\b" << "\\bother\\b" << "\\bglobal\\b" << "\\bself\\b" << "\\bio\\b" << "\\bmath\\b"
                  << "\\btable\\b" << "\\brequire\\b" << "\\bclass\\b" << "\\bsetmetatable\\b" << "\\bgetmetatable\\b"
                  << "\\b\\.\\.\\.\\b" << "\\B\\.\\.\\.\\B" << "\\bstring\\b" << "\\berror\\b" << "\\bos\\b";
    //rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
    foreach (const QString &pattern, classPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = classFormat;
        highlightingRules.append(rule);
    }

    numberFormat.setForeground(QColor(174, 129, 255));
    QStringList numberPatterns;
    numberPatterns << "\\b[0-9]+\\b" << "\\bnil\\b" << "\\btrue\\b" << "\\bfalse\\b";
    foreach (const QString &pattern, numberPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = numberFormat;
        highlightingRules.append(rule);
    }
    
    quotationFormat.setForeground(QColor(230, 219, 116));
    rule.pattern = QRegExp("\"[^\"]*(\\.[^\"]*)*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    singleQuotationFormat.setForeground(QColor(230, 219, 116));
    rule.pattern = QRegExp("\'[^\']*(\\.[^\']*)*\'");
    rule.format = singleQuotationFormat;
    highlightingRules.append(rule);
    
    escapeFormat.setForeground(QColor(174, 129, 255));
    QStringList escapePatterns;
    escapePatterns << "^\\\\n";
    foreach (const QString &pattern, escapePatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = escapeFormat;
        highlightingRules.append(rule);
    }

    singleLineCommentFormat.setFontItalic(true);
    singleLineCommentFormat.setForeground(QColor(112, 113, 94));
    rule.pattern = QRegExp("--[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setFontItalic(true);
    multiLineCommentFormat.setForeground(QColor(112, 113, 94));

    commentStartExpression = QRegExp("--\\[\\[");
    commentEndExpression = QRegExp("\\]\\]");

    //selectedWordFormat.setForeground(Qt::darkGreen);
    selectedWordFormat.setBackground(QColor(Qt::darkGreen).darker(180));
    //rule.pattern = QRegExp("\".*\"");
    rule.format = selectedWordFormat;
    //highlightingRules.append(rule);
    mSelectedWordPattern = QRegExp();
}

void Highlighter::highlightBlock(const QString &text) {
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1) {
        startIndex = commentStartExpression.indexIn(text);
    }

    while (startIndex >= 0) {
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }
}

void Highlighter::highlightSelectedWord(const QString &text) {
    highlightBlock(text);
}

void Highlighter::setSelectedWord(const QString &word) {
    if (word.length() > 2) {
        //mSelectedWordPattern = QRegExp("\b%s\b" + sanitize(word));
    } else {
        //mSelectedWordPattern = QRegExp();
    }
}

QString Highlighter::sanitize(QString &word) {
    return word.replace("\\", "\\\\");
}

void Highlighter::rehighlightLines(int line) {
    QTextBlock block = document()->findBlockByNumber(line);
    rehighlightBlock(block);
}

void Highlighter::_rehighlightLines(QList<int> &lines) {
    
}







