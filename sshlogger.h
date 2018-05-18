#ifndef SSHLOGGER_H
#define SSHLOGGER_H

#include <QObject>
#include <libssh/libssh.h>
#include <libssh/sftp.h>
#include <fcntl.h>
#include <sys/stat.h>

class SshLogger : public QObject
{
    Q_OBJECT
private:
    ssh_session mySshSession;
    sftp_session mySftpSession;
    uint64_t size;
    bool paused;
    QString host;
    int port;
    QString user;
    QString password;
    QString filename;
public:
    SshLogger(QObject *parent, QString p_host, QString p_port, QString p_user, QString p_password, QString p_filename);
    ~SshLogger();
    bool connection();
    void init();
public slots:
    void pause(bool pause);
private slots:
    void read();
signals:
    void dataAvailable(QString);
};

#endif // SSHLOGGER_H
