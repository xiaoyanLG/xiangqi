#include "xytcpserver.h"

XYTcpServer::XYTcpServer(QObject *parent)
    : QTcpServer(parent), port(45954)
{
    listen(QHostAddress::Any, port);
    client = new QTcpSocket(this);
    connect(client, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(stateChanged(QAbstractSocket::SocketState)));
    connect(client, SIGNAL(readyRead()), this, SLOT(receiveData()));
}

void XYTcpServer::connectToHost(const QHostAddress &address)
{
    client->connectToHost(address, port);
}

void XYTcpServer::receiveData()
{
    QByteArray data = client->readAll();
    qint64 type;
    qint64 size;
    QByteArray userData;
    QDataStream in(data);
    in >> type
       >> size
       >> userData;
    if (size == userData.size())
    {
        emit receiveUserData(data, type);
    }
}

void XYTcpServer::stateChanged(QAbstractSocket::SocketState state)
{
    switch (state)
    {
    case QAbstractSocket::ConnectedState:
        break;
    case QAbstractSocket::UnconnectedState:
        break;
    default:
        break;
    }
}

void XYTcpServer::writeUserData(const QByteArray &data, int type)
{
    QByteArray userDataData;
    QDataStream out(&userDataData, QIODevice::WriteOnly);

    out << qint64(type)
        << qint64(data.size())
        << data;

    client->write(userDataData);
}

void XYTcpServer::incomingConnection(int socketDescriptor)
{
    if (client->state() == QTcpSocket::UnconnectedState)
    {
        client->setSocketDescriptor(socketDescriptor);
    }
}
