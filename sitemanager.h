#ifndef SITEMANAGER_H
#define SITEMANAGER_H

#include <QDialog>
#include "ui_sitemanager.h"

class Site {
public:
    QString servername;
    QString username;
    QString password;
    QString filename;
    int port;
};
Q_DECLARE_METATYPE(Site*);

class SiteManager : public QDialog
{
    Q_OBJECT
public:
    SiteManager(QWidget *parent);
private slots:
    void on_addButton_clicked();
    void on_deleteButton_clicked();
    void on_buttonBox_accepted();
    void on_openButton_clicked();
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::SiteManager ui;
    bool addSite(QString, QString, QString, QString, int);
    void save();
signals:
    void openSite(QString, QString, QString, QString, int);
};

#endif // SITEMANAGER_H
