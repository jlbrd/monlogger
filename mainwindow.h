#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
class LogFile;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionQuit_triggered();

    void on_actionOpen_triggered();

    void on_actionOpen_jbossfrisco6_triggered();

    void on_actionOpen_jbossinterne_triggered();

    void on_actionClose_file_triggered();

    void on_actionCastor_triggered();

    void on_actionFind_triggered();

    void on_actionNext_triggered();

    void on_actionPrevious_triggered();

    void on_actionNew_triggered();

    void on_actionBergerac_triggered();

private:
    Ui::MainWindow *ui;
public slots:
    void openSite(QString servername, QString username, QString password, QString filename, int port);
};

#endif // MAINWINDOW_H
