#include "sshlogger.h"
#include "logger.h"
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#define QD qDebug() << __FILE__ << __LINE__ << ":"

#define MAX_XFER_BUF_SIZE 16384

SshLogger::SshLogger(QObject *parent, QString p_host, QString p_port, QString p_user, QString p_password, QString p_filename)
    : QObject(parent), host(p_host), user(p_user), filename(p_filename)
{
    if( !p_password.isEmpty() ) {
        password = p_password;
    }
    connect(parent, SIGNAL(pause(bool)), this, SLOT(pause(bool)));
    paused = false;
    size = 0;
    port = p_port.toInt();
    mySftpSession = 0;
}

void SshLogger::init()
{
    QTimer *timer = new QTimer(this);
    read();
    connect(timer, SIGNAL(timeout()), this, SLOT(read()));
    timer->start(5000);
}

void SshLogger::pause(bool pause)
{
    paused = pause;
}

bool SshLogger::connection()
{
    int verbosity = SSH_LOG_FUNCTIONS;
    mySshSession = ssh_new();
    if (mySshSession == NULL)
        return false;

    ssh_options_set(mySshSession, SSH_OPTIONS_HOST, host.toLatin1().data());
    ssh_options_set(mySshSession, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
    ssh_options_set(mySshSession, SSH_OPTIONS_PORT, &port);
    ssh_options_set(mySshSession, SSH_OPTIONS_USER, user.toLatin1().data());

    qDebug() << "Tentative de connexion : " << host << port << user << password << mySshSession;
    int rc = ssh_connect(mySshSession);
    if (rc != SSH_OK)
    {
        qDebug() << "Error connecting to host: " << ssh_get_error(mySshSession) << ".";
        return false;
    }
    // Authenticate ourselves
    rc = ssh_userauth_password(mySshSession, NULL, password.toLatin1().data());
    if (rc != SSH_AUTH_SUCCESS)
    {
        qDebug() << "Error authenticating with password: " << ssh_get_error(mySshSession) << ".";
        ssh_disconnect(mySshSession);
        ssh_free(mySshSession);
        return false;
    }

    mySftpSession = sftp_new(mySshSession);
    if (mySftpSession == NULL) {
        return false;
    }
    rc = sftp_init(mySftpSession);
    if (rc != SSH_OK)
    {
        qDebug() << "Error initializing SFTP session: " << sftp_get_error(mySftpSession) << ".";
        sftp_free(mySftpSession);
        return false;
    }
    return true;
}

SshLogger::~SshLogger()
{
    if( mySftpSession == NULL) {
        return;
    }
    sftp_free(mySftpSession);
    ssh_disconnect(mySshSession);
    ssh_free(mySshSession);

}

void SshLogger::read()
{
    if( paused ) {
        return;
    }
    char buffer[MAX_XFER_BUF_SIZE];
    int nbytes, rc;
    QString data;
    sftp_file file = sftp_open(mySftpSession, filename.toLatin1().data(), O_RDONLY, 0);
    if(file == NULL) {
        qDebug() << sftp_get_error(mySftpSession);
    }
    sftp_attributes attr = sftp_fstat(file);
    QD << "size" << attr->size << size;
    if(attr->size > size && size == 0) {
        // En sftp on le lit pas le fichier entièrement, uniquement les futurs ajouts
        size = attr->size;
    }
    else if(attr->size > size) {
        sftp_seek64(file, size);
        for (;;) {
            nbytes = sftp_read(file, buffer, sizeof(buffer));
            if (nbytes == 0) {
                break; // EOF
            } else if (nbytes < 0) {
                fprintf(stderr, "Error while reading file: %s\n",
                        ssh_get_error(mySftpSession));
                sftp_close(file);
                //return SSH_ERROR;
            }
            data += QString(buffer);
        }

    }
    rc = sftp_close(file);
    if (rc != SSH_OK) {
        fprintf(stderr, "Can't close the read file: %s\n",
                ssh_get_error(mySftpSession));
    }
    if(!data.isEmpty()) {
        emit dataAvailable(data);
        size += data.size();
    }
}
