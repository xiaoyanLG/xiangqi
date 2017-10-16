#include "xyqishou.h"

XYQishou *XYQishou::instance = NULL;
XYQishou::XYQishou(QObject *parent)
    : QObject(parent), type(XYQiziWidget::RED)
{
    UDPSocket = new XYUdpbroadcast(this);
    TCPServer = new XYTcpServer(this);

    connect(UDPSocket, SIGNAL(peopleUpline(QString,QHostAddress)),
            this, SIGNAL(peopleUpline(QString,QHostAddress)));
    connect(UDPSocket, SIGNAL(peopleOffline(QString,QHostAddress)),
            this, SIGNAL(peopleOffline(QString,QHostAddress)));
    connect(UDPSocket, SIGNAL(receiveData(QString,QByteArray,int)),
            this, SLOT(receiveData(QString,QByteArray,int)));

    UDPSocket->writeUplineDatagram();

    startTimer(1000);
}

XYQiziWidget *XYQishou::findQizi(XYQiziWidget::TYPE type, int times, const QPoint &lastPoint, QPoint &movePoint)
{
    XYQiziWidget *find = NULL;
    for (int i = 0; i < hong_qizis.size(); ++i)
    {
        XYQiziWidget *qizi = hong_qizis.at(i);
        if (qizi->type == type && qizi->times == times)
        {
            if (lastPoint == qizi->curPos)
            {
                find = qizi;
            }
            else if (lastPoint == qizi->getSwitchViewsPos(qizi->curPos))
            {
                movePoint = qizi->getSwitchViewsPos(movePoint);
                find = qizi;
            }
            else
            {
                return NULL;
            }
        }
    }

    for (int i = 0; i < hei_qizis.size(); ++i)
    {
        XYQiziWidget *qizi = hei_qizis.at(i);
        if (qizi->type == type && qizi->times == times)
        {
            if (lastPoint == qizi->curPos)
            {
                find = qizi;
            }
            else if (lastPoint == qizi->getSwitchViewsPos(qizi->curPos))
            {
                movePoint = qizi->getSwitchViewsPos(movePoint);
                find = qizi;
            }
            else
            {
                return NULL;
            }
        }
    }

    return find;
}

XYQishou *XYQishou::getInstance()
{
    if (instance == NULL)
    {
        instance = new XYQishou;
    }
    return instance;
}

XYQishou::~XYQishou()
{
    UDPSocket->writeOfflineDatagram();
}

void XYQishou::setQizi(const QList<XYQiziWidget *> &qizis, XYQiziWidget::SIDETYPE type)
{
    switch (type)
    {
    case XYQiziWidget::RED:
        hong_qizis = qizis;
        break;
    case XYQiziWidget::BLACK:
        hei_qizis = qizis;
        break;
    default:
        break;
    }
}

XYQiziWidget::SIDETYPE XYQishou::getSideType() const
{
    return type;
}

QIcon XYQishou::getSideIcon()
{
    if (this->type == XYQiziWidget::RED)
    {
        return QIcon(":/xiangqi/hong_jiang.png");
    }
    else if (this->type == XYQiziWidget::BLACK)
    {
        return QIcon(":/xiangqi/hei_jiang.png");
    }
}

void XYQishou::setSideType(XYQiziWidget::SIDETYPE type)
{
    this->type = type;
}

void XYQishou::switchSideType()
{
    if (this->type == XYQiziWidget::RED)
    {
        this->type = XYQiziWidget::BLACK;
    }
    else if (this->type == XYQiziWidget::BLACK)
    {
        this->type = XYQiziWidget::RED;
    }
}

void XYQishou::connectPeople(const QHostAddress &address)
{
    TCPServer->connectToHost(address);
}

void XYQishou::receiveData(const QString &from, const QByteArray &data, int type)
{
    switch (type)
    {
    case MSG:
        emit receiveMessage(from, QString::fromUtf8(data));
        break;
    case POINT:
    {
        QDataStream in(data);
        qint64 type,times;
        QPoint lastPoint, movePoint;
        bool revoked;
        in  >> type
            >> times
            >> lastPoint
            >> movePoint
            >> revoked;
        XYQiziWidget *qizi = findQizi((XYQiziWidget::TYPE)type, times, lastPoint, movePoint);
        if (qizi != NULL)
        {
            emit moveQizi(qizi, movePoint, revoked);
        }
        break;
    }
    default:
        break;
    }
}

void XYQishou::sendMessage(const QHostAddress &address, const QString &msg)
{
    UDPSocket->writeUserDatagram(address, msg.toUtf8(), MSG);
}

void XYQishou::sendQiziWithUDP(const QHostAddress &address, XYQiziWidget *qizi, const QPoint &point, bool revoked)
{
    QByteArray qiziDatagram;
    QDataStream out(&qiziDatagram, QIODevice::WriteOnly);

    out << qint64(qizi->type)
        << qint64(qizi->times)
        << qizi->curPos
        << point
        << revoked;

    UDPSocket->writeUserDatagram(address, qiziDatagram, POINT);
}

void XYQishou::sendQiziWithTCP(const QHostAddress &address, XYQiziWidget *qizi, const QPoint &point, bool revoked)
{
    QByteArray qiziDatagram;
    QDataStream out(&qiziDatagram, QIODevice::WriteOnly);

    out << qint64(qizi->type)
        << qint64(qizi->times)
        << qizi->curPos
        << point
        << revoked;

    UDPSocket->writeUserDatagram(address, qiziDatagram, POINT);
}

void XYQishou::timerEvent(QTimerEvent *event)
{
    UDPSocket->writeUplineDatagram();
}

