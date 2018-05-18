#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *document) : QSyntaxHighlighter(document)
{

}

void Highlighter::highlightBlock(const QString &text)
{
    if(level == "ERROR") {
        setFormat(0, text.length(), Qt::red);
    }
    else if(level == "INFO") {
        setFormat(0, text.length(), Qt::darkGreen);
    }
    else {
        setFormat(0, text.length(), Qt::black);
    }
}

void Highlighter::setLevel(QString value)
{
    level = value;
}
