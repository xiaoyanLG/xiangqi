#ifndef XYQISHOU_H
#define XYQISHOU_H

#include <QObject>
#include <QIcon>
#include "xyudpbroadcast.h"
#include "xytcpserver.h"
#include "xyqiziwidget.h"
#include "xyqipanwidget.h"

class XYQishou : public QObject
{
    Q_OBJECT
public:
    enum MSGTYPE{MSG, REQUEST, ACCEPT, REJECT, SIDE, POINT};
    static XYQishou *getInstance();
    ~XYQishou();

    void setQipan(XYQipanWidget *qipan);
    XYQiziWidget::SIDETYPE getSideType() const;
    QIcon getSideIcon();
    void setSideType(XYQiziWidget::SIDETYPE type);
    void switchSideType();

signals:
    void showMessages(const QString &msg, int times);
    void sideChanged();
    void peopleUpline(const QString &name, const QHostAddress &address);
    void peopleOffline(const QString &name, const QHostAddress &address);
    void receiveMessage(const QString &from, const QString &msg);
    void moveQizi(XYQiziWidget *qizi, const QPoint &point, bool revoked);


public slots:
    void connectPeople(const QHostAddress &address);
    void receiveUserData(const QString &from, const QByteArray &data, int type);
    void sendMessage(const QHostAddress &address, const QString &msg);
    void sendQizi(const QHostAddress &address,
                  XYQiziWidget *qizi,
                  const QPoint &point,
                  bool revoked);
    void sendQiziWithUDP(const QHostAddress &address,
                         XYQiziWidget *qizi,
                         const QPoint &point,
                         bool revoked);
    void sendQiziWithTCP(const QHostAddress &address,
                         XYQiziWidget *qizi,
                         const QPoint &point,
                         bool revoked);
    void peopleResponse(QAbstractSocket::SocketState state);

protected:
    void timerEvent(QTimerEvent *event);

private:
    explicit XYQishou(QObject *parent = 0);

private:
    static XYQishou       *instance;
    XYQipanWidget         *qipan;
    QString                opponentQishou;
    XYUdpbroadcast        *UDPSocket;
    XYTcpServer           *TCPServer;
    XYQiziWidget::SIDETYPE type;

};

#endif // XYQISHOU_H
