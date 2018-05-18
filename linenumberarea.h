#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

#include <QWidget>
#include "myplaintextedit.h"

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(MyPlainTextEdit *editor);

    QSize sizeHint();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    MyPlainTextEdit *codeEditor;
};

#endif // LINENUMBERAREA_H
