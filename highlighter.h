#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>

class Highlighter : public QSyntaxHighlighter
{
private:
    QString level;
public:
    Highlighter(QTextDocument *document);
    void highlightBlock(const QString &text);
    void setLevel(QString value);
};

#endif // HIGHLIGHTER_H
