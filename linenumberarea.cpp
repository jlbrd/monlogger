#include "linenumberarea.h"

LineNumberArea::LineNumberArea(MyPlainTextEdit *editor) : QWidget(editor)
{
    codeEditor = editor;
}

QSize LineNumberArea::sizeHint()
{
    return QSize(codeEditor->lineNumberAreaWidth(), 0);
}

void LineNumberArea::paintEvent(QPaintEvent *event)
{
    codeEditor->lineNumberAreaPaintEvent(event);
}
