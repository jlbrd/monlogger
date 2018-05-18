#include "sitemanager.h"
#include <QListWidget>
#include <QSettings>
#include <QDebug>
#define QD qDebug() << __FILE__ << __LINE__ << ":"

SiteManager::SiteManager(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);
    QSettings settings("monlogger.ini", QSettings::IniFormat);
    int numItem = 0;
    while(1) {
        QString group = "site"+QString::number(numItem);
        settings.beginGroup(group);
        QString servername = settings.value("servername").toString();
        QString username = settings.value("username").toString();
        QString password = settings.value("password").toString();
        QString filename = settings.value("filename").toString();
        int port = settings.value("port").toInt();
        settings.endGroup();
        if( !addSite(servername, username, password, filename, port) ) {
            break;
        }

        numItem++;
    }
}

void SiteManager::on_addButton_clicked()
{
    addSite(ui.servername->text(), ui.username->text(), ui.password->text(), ui.filename->text(), ui.port->value());
}

void SiteManager::on_deleteButton_clicked()
{
    QListWidgetItem* item = ui.listWidget->currentItem();
    if(item == 0) {
        return;
    }
    delete item;
}

void SiteManager::on_buttonBox_accepted()
{
    save();
    accept();
}

void SiteManager::on_openButton_clicked()
{
    emit openSite(ui.servername->text(), ui.username->text(), ui.password->text(), ui.filename->text(), ui.port->value());
}

bool SiteManager::addSite(QString servername, QString username, QString password, QString filename, int port)
{
    if(username.isEmpty() || servername.isEmpty()) {
        return false;
    }
    QListWidgetItem* item = new QListWidgetItem(ui.listWidget);
    item->setText(username + "@" + servername);
    Site* site = new Site;
    site->servername = servername;
    site->username = username;
    site->password = password;
    site->filename = filename;
    site->port = port;
    QVariant variant;
    variant.setValue(site);
    item->setData(Qt::UserRole, variant);
    ui.listWidget->addItem(item);
    return true;
}

void SiteManager::save()
{
    QSettings settings("monlogger.ini", QSettings::IniFormat);
    QD << settings.fileName();
    for(int numItem = 0; numItem < ui.listWidget->count(); numItem++) {
        QListWidgetItem* item = ui.listWidget->item(numItem);
        QVariant variant = item->data(Qt::UserRole);
        Site* site = variant.value<Site*>();
        settings.beginGroup("site"+QString::number(numItem));
        settings.setValue("title", site->username+"@"+site->servername);
        settings.setValue("servername", site->servername);
        settings.setValue("username", site->username);
        settings.setValue("password", site->password);
        settings.setValue("filename", site->filename);
        settings.setValue("port", site->port);
        settings.endGroup();
    }
}



void SiteManager::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    on_listWidget_itemClicked(item);
    on_openButton_clicked();
}

void SiteManager::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QVariant variant = item->data(Qt::UserRole);
    Site* site = variant.value<Site*>();
    ui.servername->setText(site->servername);
    ui.username->setText(site->username);
    ui.password->setText(site->password);
    ui.filename->setText(site->filename);
    ui.port->setValue(site->port);
}
