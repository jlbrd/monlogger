#ifndef MYPLAINTEXTEDIT_H
#define MYPLAINTEXTEDIT_H

#include <QPlainTextEdit>
#include <QTextBlock>
#include <QObject>
#include "ui_findwidget.h"

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
class LineNumberArea;
class Logger;
class Highlighter;

class MyPlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    MyPlainTextEdit(QWidget *parent);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    void insertText(QString newText);
    void clear();
    void refiltreDepuisDebut();
protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);
public slots:
    void filtreTout();
    void slotFind(Ui::FindWidget uiFind, QString ttf=QString(), QTextDocument::FindFlags options=0, bool fromButton=false);
private:
    Logger *logger;
    QWidget *lineNumberArea;
    QString text;
    QString newText;
    bool optionActive(QString key);
    bool enFiltrage;
    bool enPause;
    QTextBlock lastBlock;
    bool blockVisible;
    void filtre();
    Highlighter* highlighter;
};

#endif // MYPLAINTEXTEDIT_H
