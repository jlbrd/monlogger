#include <QPainter>
#include <QTextBlock>
#include <QDebug>
#include <QApplication>
#include <QTextCursor>
#include <QScrollBar>
#include "myplaintextedit.h"
#include "linenumberarea.h"
#include "logger.h"
#include "highlighter.h"

#define QD qDebug() << __FILE__ << __LINE__ << ":"

MyPlainTextEdit::MyPlainTextEdit(QWidget *parent) : QPlainTextEdit(parent)
{
    logger = (Logger *)parent;
    lastBlock = QTextBlock();
    lastBlock.setRevision(-1);
    enFiltrage = false;
    blockVisible = true;
    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    highlighter = new Highlighter(this->document());
    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

}

void MyPlainTextEdit::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignLeft, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

int MyPlainTextEdit::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 13 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;

}

void MyPlainTextEdit::insertText(QString p_newText)
{
    newText += p_newText.toLatin1();
    //QD << "enFiltrage" << enFiltrage;
    if(!enFiltrage) {
        this->moveCursor(QTextCursor::End);
        this->textCursor().insertText(newText);
        filtre();
        newText = "";
    }
}
void MyPlainTextEdit::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));

}

void MyPlainTextEdit::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void MyPlainTextEdit::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);

}

void MyPlainTextEdit::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void MyPlainTextEdit::clear()
{
    lastBlock = QTextBlock();
    lastBlock.setRevision(-1);
    blockVisible = false;
    QPlainTextEdit::clear();
}

void MyPlainTextEdit::filtreTout() {
    lastBlock = QTextBlock();
    lastBlock.setRevision(-1);
    blockVisible = false;
    filtre();
}

void MyPlainTextEdit::slotFind(Ui::FindWidget uiFind, QString ttf, QTextDocument::FindFlags options, bool fromButton)
{
    QTextDocument *doc = document();
    QString oldText = uiFind.editFind->text();
    QTextCursor c = textCursor();
    QPalette p = uiFind.editFind->palette();
    p.setColor(QPalette::Active, QPalette::Base, Qt::white);

    if (c.hasSelection())
    {
        if ( options & QTextDocument::FindBackward || fromButton)
            c.setPosition(c.position(), QTextCursor::MoveAnchor);
        else
            c.setPosition(c.anchor(), QTextCursor::MoveAnchor);
    }


    QTextCursor newCursor = c;

    if (!ttf.isEmpty())
    {
        newCursor = doc->find(ttf, c, options);
        uiFind.labelWrapped->hide();

        if (newCursor.isNull())
        {
            QTextCursor ac(doc);
            ac.movePosition(options & QTextDocument::FindBackward
                            ? QTextCursor::End : QTextCursor::Start);
            newCursor = doc->find(ttf, ac, options);
            if (newCursor.isNull())
            {
                p.setColor(QPalette::Active, QPalette::Base, QColor(255, 102, 102));
                newCursor = c;
            }
            else
            {
                uiFind.labelWrapped->show();
                //uiFind.reached->setText(tr("Reached end of page, continued from top"));
                uiFind.labelWrapped->show();
            }
        }
    }
    setTextCursor(newCursor);
    uiFind.editFind->setPalette(p);

}

void MyPlainTextEdit::filtre()
{
    enFiltrage = true;
    QStringList levels = logger->levels();
    QTextBlock block;
    //QD << "revision" << lastBlock.revision();
    if( lastBlock.revision() == -1) {
        QTextCursor cursor = textCursor();
        cursor.setPosition(0);
        block = cursor.block();
    }
    else {
        block = lastBlock;
    }
    if(!block.isValid()) {
        enFiltrage = false;
        return;
    }
    if(block.isValid()) {
        //QD << "debut filtrage ligne " << block.blockNumber();
        do {
            lastBlock = block;
            //QD << "ligneActuelle" << lastBlock.blockNumber() << "isValide" << lastBlock.isValid();
            QString key;
            for(int pos = 1; pos < 4 && !levels.contains(key); pos++) {
                key = block.text().section(QRegExp("[\t ]+"), pos, pos);
            }

            //QD << "optionActive" << key << optionActive(key);
            if(optionActive(key)) {
                blockVisible = true;
                highlighter->setLevel(key);

            }
            else if(levels.contains(key)) {
                blockVisible = false;
            }
            highlighter->rehighlightBlock(block);
            //QD << "blockVisible " << blockVisible <<" : " << block.text();
            block.setVisible(blockVisible);
            block = block.next();
        } while (block.isValid());
        lastBlock.setRevision(1);
    }
    enFiltrage = false;
    document()->markContentsDirty(0, 1000000);
    update();
    viewport()->update();
    verticalScrollBar()->update();
    //QD << "fin filtrage ligne " << lastBlock.blockNumber();
}

bool MyPlainTextEdit::optionActive(QString key)
{
    return logger->levelChecked(key);
}

