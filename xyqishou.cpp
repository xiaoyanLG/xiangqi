#include "xyqishou.h"
#include "mainwindow.h"
#include <QMessageBox>

XYQishou *XYQishou::instance = NULL;
XYQishou::XYQishou(QObject *parent)
    : QObject(parent), type(XYQiziWidget::RED), qipan(NULL)
{
    UDPSocket = new XYUdpbroadcast(this);
    TCPServer = new XYTcpServer(this);

    connect(UDPSocket, SIGNAL(peopleUpline(QString,QHostAddress)),
            this, SIGNAL(peopleUpline(QString,QHostAddress)));
    connect(UDPSocket, SIGNAL(peopleOffline(QString,QHostAddress)),
            this, SIGNAL(peopleOffline(QString,QHostAddress)));
    connect(UDPSocket, SIGNAL(receiveUserData(QString,QByteArray,int)),
            this, SLOT(receiveUserData(QString,QByteArray,int)));

    connect(TCPServer, SIGNAL(receiveUserData(QString,QByteArray,int)),
            this, SLOT(receiveUserData(QString,QByteArray,int)));
    connect(TCPServer, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(peopleResponse(QAbstractSocket::SocketState)));
    UDPSocket->writeUplineDatagram();

    startTimer(1000);
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

void XYQishou::setQipan(XYQipanWidget *qipan)
{
    this->qipan = qipan;
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
    if (TCPServer->isConnected())
    {
        if (QMessageBox::question(MainWindow::getInstance(), QString::fromStdWString(L"提示"),
                              QString::fromStdWString(L"你正在和 %1 对战，是否退出？").arg(opponentQishou))
                == QMessageBox::Yes)
        {
            opponentQishou.clear();
            TCPServer->abort();
        }
    }
    else
    {
        TCPServer->connectToHost(address);
    }
}

void XYQishou::receiveUserData(const QString &from, const QByteArray &data, int type)
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
        QByteArray key;
        bool revoked;
        in  >> type
            >> times
            >> lastPoint
            >> movePoint
            >> key
            >> revoked;
        if (qipan != NULL)
        {
            XYQiziWidget *qizi = qipan->findQizi(key, (XYQiziWidget::TYPE)type, times, lastPoint, movePoint);
            if (qizi != NULL)
            {
                emit moveQizi(qizi, movePoint, revoked);
            }
        }
        break;
    }
    case REQUEST:
    {
        if (data.isEmpty())
        {
            if (QMessageBox::question(MainWindow::getInstance(), from,
                                  QString::fromStdWString(L"请求对战，是否接受？"),
                                  QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
            {
                TCPServer->writeUserData(QByteArray(), ACCEPT);
                opponentQishou = from;
                if (qipan != NULL)
                {
                    qipan->layoutQizi(true);
                }
            }
            else
            {
                TCPServer->writeUserData(QByteArray(), REJECT);
                TCPServer->abort();
                break;
            }

            QByteArray sideType;
            QDataStream in(&sideType, QIODevice::WriteOnly);
            if (QMessageBox::question(MainWindow::getInstance(), from, QString::fromStdWString(L"请选择棋方，红（yes）还是黑（no）？"),
                                  QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
            {
                in << qint64(sizeof(qint64))
                   << qint64(XYQiziWidget::BLACK);
                this->type = XYQiziWidget::RED;
            }
            else
            {
                in << qint64(sizeof(qint64))
                   << qint64(XYQiziWidget::RED);
                this->type = XYQiziWidget::BLACK;
            }
            emit sideChanged();
            TCPServer->writeUserData(sideType, SIDE);
        }
        break;
    }
    case ACCEPT:
    {
        if (data.isEmpty())
        {
            opponentQishou = from;
            if (qipan != NULL)
            {
                qipan->layoutQizi(true);
            }
            emit showMessages(QString("%1: %2").arg(from).arg(QString::fromStdWString(L"接受对战，请等待对方选择棋方！")), 0);
        }
        break;
    }
    case REJECT:
    {
        if (data.isEmpty())
        {
            emit showMessages(QString("%1: %2").arg(from).arg(QString::fromStdWString(L"拒绝了对战请求！")), 5000);
        }
        break;
    }
    case SIDE:
    {
        QDataStream out(data);
        qint64 size = 0;
        qint64 type = 0;
        out >> size >> type;
        if (size == sizeof(qint64))
        {
            this->type = (XYQiziWidget::SIDETYPE)type;
            emit sideChanged();
            if (this->type == XYQiziWidget::RED)
            {
                emit showMessages(QString("%1: %2")
                                 .arg(from)
                                 .arg(QString::fromStdWString(L"选择%1方！").arg(QString::fromStdWString(L"黑"))), 0);
            }
            else
            {
                emit showMessages(QString("%1: %2")
                                 .arg(from)
                                 .arg(QString::fromStdWString(L"选择%1方！").arg(QString::fromStdWString(L"红"))), 5000);
            }
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

void XYQishou::sendQizi(const QHostAddress &address,
                        XYQiziWidget *qizi,
                        const QPoint &point,
                        const QByteArray &key,
                        bool revoked)
{
    if (TCPServer->isConnected())
    {
        sendQiziWithTCP(address, qizi, point, key, revoked);
    }
    else
    {
        sendQiziWithUDP(address, qizi, point, key, revoked);
    }
}

void XYQishou::sendQiziWithUDP(const QHostAddress &address,
                               XYQiziWidget *qizi,
                               const QPoint &point,
                               const QByteArray &key,
                               bool revoked)
{
    QByteArray qiziDatagram;
    QDataStream out(&qiziDatagram, QIODevice::WriteOnly);

    out << qint64(qizi->type)
        << qint64(qizi->times)
        << qizi->curPos
        << point
        << key
        << revoked;

    UDPSocket->writeUserDatagram(address, qiziDatagram, POINT);
}

void XYQishou::sendQiziWithTCP(const QHostAddress &address,
                               XYQiziWidget *qizi,
                               const QPoint &point,
                               const QByteArray &key,
                               bool revoked)
{
    QByteArray qiziDatagram;
    QDataStream out(&qiziDatagram, QIODevice::WriteOnly);

    out << qint64(qizi->type)
        << qint64(qizi->times)
        << qizi->curPos
        << point
        << key
        << revoked;

    UDPSocket->writeUserDatagram(address, qiziDatagram, POINT);
}

void XYQishou::peopleResponse(QAbstractSocket::SocketState state)
{
    switch (state)
    {
    case QAbstractSocket::ConnectedState:
    {
        if (TCPServer->isSender())
        {
            TCPServer->writeUserData(QByteArray(), REQUEST);
        }
        break;
    }
    case QAbstractSocket::UnconnectedState:
        if (!opponentQishou.isEmpty())
        {
            emit showMessages(opponentQishou + QString::fromStdWString(L" 逃跑了！"), 5000);
        }
        opponentQishou.clear();
        break;
    default:
        break;
    }
}

void XYQishou::timerEvent(QTimerEvent *event)
{
    UDPSocket->writeUplineDatagram();
}

