#ifndef LOCALSOCKETIPCCLIENT_HPP
#define LOCALSOCKETIPCCLIENT_HPP

#include <QtNetwork/QLocalSocket>
#include <QLocalSocket>
#include <QLocalServer>
#include <QTextStream>
#include <qdatastream.h>

class LocalSocketIpcClient : public QObject
{
    Q_OBJECT
public:
    LocalSocketIpcClient(QString remoteServername, QObject *parent = 0);
    ~LocalSocketIpcClient();

signals:
    void serverResponse(QString);

public slots:
    void send_MessageToServer(QString message);

    void socket_connected();
    void socket_disconnected();

    void socket_readReady();
    void socket_error(QLocalSocket::LocalSocketError);

private:
    QLocalSocket*   m_socket;
    quint16 m_blockSize;
    QString m_message;
    QString m_serverName;
};

#endif // LOCALSOCKETIPCCLIENT_HPP
