#include "localsocketipcclient.h"
#include <QDebug>

LocalSocketIpcClient::LocalSocketIpcClient(QString remoteServername, QObject *parent) :
        QObject(parent) {

    m_socket = new QLocalSocket(this);
    m_serverName = remoteServername;

    connect(m_socket, SIGNAL(connected()), this, SLOT(socket_connected()));
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(socket_disconnected()));

    connect(m_socket, SIGNAL(readyRead()), this, SLOT(socket_readReady()));
    connect(m_socket, SIGNAL(error(QLocalSocket::LocalSocketError)),
            this, SLOT(socket_error(QLocalSocket::LocalSocketError)));

}

LocalSocketIpcClient::~LocalSocketIpcClient() {
    m_socket->abort();
    delete m_socket;
    m_socket = NULL;
}

void LocalSocketIpcClient::send_MessageToServer(QString message) {
    m_socket->abort();
    m_message = message;
    m_socket->connectToServer(m_serverName);
}


void LocalSocketIpcClient::socket_connected(){
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << m_message.toUtf8().data();
    out.device()->seek(0);
    m_socket->write(block);
    m_socket->flush();
}

void LocalSocketIpcClient::socket_disconnected() {

}


void LocalSocketIpcClient::socket_readReady() {
    QDataStream in(this->m_socket);
    in.setVersion(QDataStream::Qt_4_0);
    if (this->m_socket->bytesAvailable() < (int)sizeof(quint16)) {
        return;
    }
    QString message;
    in >> message;
    emit serverResponse(message);
}

void LocalSocketIpcClient::socket_error(QLocalSocket::LocalSocketError error) {
    qDebug() << "socket_error : " << error;
}
