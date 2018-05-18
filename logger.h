#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QWidget>
#include "ui_logfile.h"

class QStandardItemModel;
class Find;

class Logger : public QWidget
{
    Q_OBJECT
public:
    Logger(QWidget *parent);
    bool init(QString p_filename);
    bool init(QString host, QString port, QString user, QString password, QString filename);
    bool onPause() { return paused; }
    bool info();
    bool warn();
    bool debug();
    bool error();
    bool levelChecked(QString level);
    QStringList levels();
    void find();
    void findPrevious();
    void findNext();
private:
    Ui::LogFile *ui;
    QString filename;
    qint64 size;
    QStandardItemModel* comboLevelsModel;
    bool paused;
    void enableCheckbox(bool enable);
    void initComboLevels();
    Find* finder;
private slots:
    void read();
    void on_refresh_clicked();
    void on_stop_clicked();
    void on_clear_clicked();

public slots:
    void dataAvailable(QString);
signals:
    void pause(bool);
    void clear();
};

#endif // LOGGER_H
