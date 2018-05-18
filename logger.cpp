#include "logger.h"
#include "sshlogger.h"
#include "find.h"
#include "simplecrypt.h"
#include <QFile>
#include <QFileInfo>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QPair>
#include <QCryptographicHash>
#include <QDebug>
#define QD qDebug() << __FILE__ << __LINE__ << ":"

Logger::Logger(QWidget *parent)
    : QWidget(parent), ui(new Ui::LogFile)
{
    ui->setupUi(this);
    initComboLevels();
    finder = new Find(ui->findWidgetContainer, ui->plainTextEdit);
    paused = false;
    size = -1;
}

bool Logger::init(QString p_filename)
{
    filename = p_filename;
    connect(ui->refresh, SIGNAL(clicked()), this, SLOT(read()));
    QTimer *timer = new QTimer(this);
    //read();
    connect(timer, SIGNAL(timeout()), this, SLOT(read()));
    timer->start(1000);
    qlonglong masterPassword = QCryptographicHash::hash(("myPassword"), QCryptographicHash::Md5).toLongLong();
    SimpleCrypt crypto(masterPassword);
    QString crypt = crypto.encryptToString(QString("lamotdepasse"));
    QD << crypt << SimpleCrypt(masterPassword).decryptToString(crypt) ;
    //SimpleCrypt crypto2(Q_UINT64_C(0x0c2ad4a4acb9f023));
    return true;
}

bool Logger::init(QString host, QString port, QString user, QString password, QString filename)
{
    SshLogger *sshLogger = new SshLogger(this, host, port, user, password, filename);
    connect(sshLogger, SIGNAL(dataAvailable(QString)), this, SLOT(dataAvailable(QString)));
    if( !sshLogger->connection() ) {
        return false;
    }
    sshLogger->init();
    return true;
}

void Logger::initComboLevels()
{
    comboLevelsModel = new QStandardItemModel(1, 1, ui->comboLevels);
    QList<QPair<QString, Qt::CheckState>> levels;
    levels.append(qMakePair(QString("TRACE"), Qt::Unchecked));
    levels.append(qMakePair(QString("DEBUG"), Qt::Checked));
    levels.append(qMakePair(QString("INFO"), Qt::Unchecked));
    levels.append(qMakePair(QString("WARN"), Qt::Unchecked));
    levels.append(qMakePair(QString("ERROR"), Qt::Checked));
    levels.append(qMakePair(QString("FATAL"), Qt::Checked));
    for(int row = 0; row < levels.size(); row++) {
        QPair<QString, Qt::CheckState> level = levels.at(row);
        QStandardItem* item = new QStandardItem(level.first);
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setData(level.second, Qt::CheckStateRole);
        comboLevelsModel->setItem(row, 0, item);
    }
    connect(comboLevelsModel, SIGNAL(dataChanged ( const QModelIndex&, const QModelIndex&)), ui->plainTextEdit, SLOT(filtreTout()));
    ui->comboLevels->setModel(comboLevelsModel);
}

bool Logger::levelChecked(QString level)
{
    for(int row = 0; row < comboLevelsModel->rowCount(); row++) {
        QStandardItem* item = comboLevelsModel->item(row);
        bool checked = (item->checkState() == Qt::Checked);
        QString text = item->text();
        if(item->text() == level) {
            return checked;
        }
    }
    return false;
}

QStringList Logger::levels()
{
    QStringList levels;
    for(int row = 0; row < comboLevelsModel->rowCount(); row++) {
        QStandardItem* item = comboLevelsModel->item(row);
        QString text = item->text();
        levels << text;
    }
    return levels;
}

void Logger::find()
{
    this->finder->find();
}

void Logger::findPrevious()
{
    this->finder->findPrevious();
}

void Logger::findNext()
{
    this->finder->findNext();
}

void Logger::enableCheckbox(bool enable)
{
    paused = !enable;
}

void Logger::read()
{
    if(paused) {
        QD<< "En pause";
        return;
    }
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    //QD<<file.size();
    if(size == -1) {
        size = file.size();
        return;
    }
    if(file.size() > size) {
        file.seek(size);
        QString data;
        data = file.read(file.size() - size);
        size = file.size();
        dataAvailable(data);
    }
    file.close();
}

void Logger::dataAvailable(QString data)
{
    ui->plainTextEdit->insertText(data.toLatin1());
}

void Logger::on_refresh_clicked()
{

}

void Logger::on_stop_clicked()
{
    paused = !paused;
    emit pause(paused);
    if(paused) {
        ui->stop->setIcon(QIcon(":/Toolbar/player_play.png"));
    }
    else {
        ui->stop->setIcon(QIcon(":/Toolbar/player_pause.png"));
    }

}

void Logger::on_clear_clicked()
{
    ui->plainTextEdit->clear();
}


