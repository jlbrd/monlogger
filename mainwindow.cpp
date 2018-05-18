#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_logfile.h"
#include "sshlogger.h"
#include "sitemanager.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include "logger.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug() << QDir::currentPath();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    close();
}

void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Open Image"), "C:/bin/jboss-eap-6.3/jboss-as/standalone/log", tr("Text files (*.txt *.log)"));
    //QString filename = "c:/c.txt";
    if( filename == "") {
        return;
    }
    Logger *logger = new Logger(this);
    logger->init(filename);
    ui->tabWidget->addTab(logger, filename);
}

void MainWindow::on_actionOpen_jbossfrisco6_triggered()
{
}

void MainWindow::on_actionOpen_jbossinterne_triggered()
{
}

void MainWindow::on_actionClose_file_triggered()
{
    if(ui->tabWidget->currentWidget() != 0) {
        delete ui->tabWidget->currentWidget();
    }
}

void MainWindow::on_actionCastor_triggered()
{
}

void MainWindow::on_actionFind_triggered()
{
    if(ui->tabWidget->currentWidget() != 0) {
        Logger *logger = (Logger *)ui->tabWidget->currentWidget();
        logger->find();
    }
}

void MainWindow::on_actionNext_triggered()
{
    if(ui->tabWidget->currentWidget() != 0) {
        Logger *logger = (Logger *)ui->tabWidget->currentWidget();
        logger->findNext();
    }
}

void MainWindow::on_actionPrevious_triggered()
{
    if(ui->tabWidget->currentWidget() != 0) {
        Logger *logger = (Logger *)ui->tabWidget->currentWidget();
        logger->findPrevious();
    }
}

void MainWindow::on_actionNew_triggered()
{
    SiteManager siteManager(this);
    connect(&siteManager, SIGNAL(openSite(QString, QString, QString, QString, int)), this, SLOT(openSite(QString, QString, QString, QString, int)));
    siteManager.exec();
}

void MainWindow::on_actionBergerac_triggered()
{
}

void MainWindow::openSite(QString servername, QString username, QString password, QString filename, int port)
{
    Logger *logger = new Logger(this);
    bool initOK = logger->init(servername, QString::number(port), username, password, filename);
    if(initOK) {
        ui->tabWidget->addTab(logger, servername);
    } else {
        delete logger;
        QMessageBox::critical(this, "Connexion", "Impossible de se connecter sur " + username+"@"+servername);
    }
}
