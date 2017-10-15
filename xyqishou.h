#ifndef XYQISHOU_H
#define XYQISHOU_H

#include <QObject>
#include "xyudpbroadcast.h"
#include "xytcpsocket.h"
#include "xyqiziwidget.h"

class XYQishou : public QObject
{
    Q_OBJECT
public:
    enum MSGTYPE{MSG, START, SIDE, POINT, END};
    static XYQishou *getInstance();
    ~XYQishou();

    void setQizi(const QList<XYQiziWidget *> &qizis, XYQiziWidget::SIDETYPE type);
    XYQiziWidget::SIDETYPE getSideType() const;
    void setSideType(XYQiziWidget::SIDETYPE type);

signals:
    void peopleUpline(const QString &name, const QHostAddress &address);
    void peopleOffline(const QString &name, const QHostAddress &address);
    void receiveMessage(const QString &from, const QString &msg);
    void moveQizi(XYQiziWidget *qizi, const QPoint &point);


public slots:
    void receiveData(const QString &from, const QByteArray &data, int type);
    void sendMessage(const QHostAddress &address, const QString &msg);
    void sendQizi(const QHostAddress &address, XYQiziWidget *qizi, const QPoint &point);

protected:
    void timerEvent(QTimerEvent *event);

private:
    explicit XYQishou(QObject *parent = 0);
    XYQiziWidget *findQizi(XYQiziWidget::TYPE type,
                           int times,
                           const QPoint &lastPoint,
                           QPoint &movePoint);

private:
    static XYQishou       *instance;
    QList<XYQiziWidget *>  hong_qizis;
    QList<XYQiziWidget *>  hei_qizis;
    XYUdpbroadcast        *UDPSocket;
    XYTcpSocket           *TCPSocket;
    XYQiziWidget::SIDETYPE type;

};

#endif // XYQISHOU_H
