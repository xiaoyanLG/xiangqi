#include "xytcpsocket.h"

XYTcpSocket::XYTcpSocket(QObject *parent)
    : QObject(parent), port(45954)
{
    server = new QTcpServer(this);
    server->listen(QHostAddress::Any, port);
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnect()));
    client = new QTcpSocket(this);
    connect(client, SIGNAL(readyRead()), this, SLOT(receiveData()));
}

void XYTcpSocket::connectToHost(const QHostAddress &address)
{
    client->connectToHost(address, port);
}

void XYTcpSocket::newConnect()
{

}

void XYTcpSocket::receiveData()
{

}
