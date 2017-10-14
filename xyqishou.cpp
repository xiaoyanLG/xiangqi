#include "xyqishou.h"

XYQishou::XYQishou(QObject *parent)
    : QObject(parent)
{
    socket = new XYUdpbroadcast(this);

    connect(socket, SIGNAL(peopleUpline(QString,QHostAddress)),
            this, SIGNAL(peopleUpline(QString,QHostAddress)));
    connect(socket, SIGNAL(peopleOffline(QString,QHostAddress)),
            this, SIGNAL(peopleOffline(QString,QHostAddress)));
    connect(socket, SIGNAL(receiveData(QString,QString)),
            this, SIGNAL(receiveData(QString,QString)));

    socket->writeUplineDatagram();

    startTimer(1000);
}

XYQishou::~XYQishou()
{
    socket->writeOfflineDatagram();
}

void XYQishou::sendMessage(const QHostAddress &address, const QString &msg)
{
    socket->writeUserDatagram(address, msg);
}

void XYQishou::timerEvent(QTimerEvent *event)
{
    socket->writeUplineDatagram();
}

