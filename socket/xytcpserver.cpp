#include "xytcpserver.h"
#include "xyudpbroadcast.h"
#include <QDataStream>

XYTcpServer::XYTcpServer(QObject *parent)
    : QTcpServer(parent), port(45954)
{
    listen(QHostAddress::Any, port);
    userName = XYUdpbroadcast::getUserName();
    client = new QTcpSocket(this);
    connect(client, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SIGNAL(stateChanged(QAbstractSocket::SocketState)));
    connect(client, SIGNAL(readyRead()), this, SLOT(receiveData()));
}

bool XYTcpServer::isSender()
{
    return sender;
}

bool XYTcpServer::isConnected()
{
    return client->state() == QTcpSocket::ConnectedState;
}

void XYTcpServer::abort()
{
    client->abort();
}

void XYTcpServer::connectToHost(const QHostAddress &address)
{
    sender = true;
    client->connectToHost(address, port);
}

void XYTcpServer::receiveData()
{
    QByteArray data = client->readAll();
    qint64 type;
    qint64 size;
    QByteArray user;
    QByteArray userData;
    QDataStream in(data);
    in >> type
       >> size
       >> user;

    // 大小不符合，数据接收错误
    if (size == user.size())
    {
        in >> size >> userData;
        if (userData.size() != size)
        {
            return;
        }
        emit receiveUserData(QString::fromUtf8(user), userData, type, false);
    }
}

void XYTcpServer::writeUserData(const QByteArray &data, int type)
{
    QByteArray userDataData;
    QDataStream out(&userDataData, QIODevice::WriteOnly);

    out << qint64(type)
        << qint64(userName.toUtf8().size())
        << userName.toUtf8()
        << qint64(data.size())
        << data;

    client->write(userDataData);
    client->waitForBytesWritten(3000);
}

void XYTcpServer::incomingConnection(int socketDescriptor)
{
    if (client->state() == QTcpSocket::UnconnectedState)
    {
        sender = false;
        client->setSocketDescriptor(socketDescriptor);
    }
}
