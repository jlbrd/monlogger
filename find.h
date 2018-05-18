#ifndef FIND_H
#define FIND_H

#include <QWidget>
#include "ui_findwidget.h"
#include <QTimer>
#include "myplaintextedit.h"

class Find : public QWidget
{
    Q_OBJECT
public:
    explicit Find(QWidget *parent, MyPlainTextEdit *myPlainTextEdit);
    void find();
private:
    Ui::FindWidget uiFindWidget;
    QTimer* autoHideFindWidgetTimer;
    bool findBackward;
    MyPlainTextEdit *m_myPlainTextEdit;
signals:

private slots:
    void findWidget_textChanged(QString text="", bool fromButton=false);
public slots:
    void findPrevious();
    void findNext();
};

#endif // FIND_H
