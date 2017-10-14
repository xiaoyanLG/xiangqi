#include "xyqishou.h"

XYQishou::XYQishou(QObject *parent)
    : QObject(parent)
{
    socket = new XYUdpbroadcast(this);

    connect(socket, SIGNAL(peopleUpline(QString,QHostAddress)),
            this, SIGNAL(peopleUpline(QString,QHostAddress)));
    connect(socket, SIGNAL(peopleOffline(QString,QHostAddress)),
            this, SIGNAL(peopleOffline(QString,QHostAddress)));
    connect(socket, SIGNAL(receiveData(QString,QByteArray,int)),
            this, SLOT(receiveData(QString,QByteArray,int)));

    socket->writeUplineDatagram();

    startTimer(1000);
}

XYQishou::~XYQishou()
{
    socket->writeOfflineDatagram();
}

void XYQishou::receiveData(const QString &from, const QByteArray &data, int type)
{
    switch (type)
    {
    case MSG:
        emit receiveMessage(from, QString::fromUtf8(data));
        break;
    case POINT:
        break;
    default:
        break;
    }
}

void XYQishou::sendMessage(const QHostAddress &address, const QString &msg)
{
    socket->writeUserDatagram(address, msg.toUtf8(), MSG);
}

void XYQishou::sendQizi(const QHostAddress &address, XYQiziWidget *qizi)
{

}

void XYQishou::timerEvent(QTimerEvent *event)
{
    socket->writeUplineDatagram();
}

