#ifndef XYQISHOU_H
#define XYQISHOU_H

#include <QObject>
#include "xyudpbroadcast.h"
#include "xyqiziwidget.h"

class XYQishou : public QObject
{
    Q_OBJECT
public:
    enum MSGTYPE{MSG, POINT};
    explicit XYQishou(QObject *parent = 0);
    ~XYQishou();

signals:
    void peopleUpline(const QString &name, const QHostAddress &address);
    void peopleOffline(const QString &name, const QHostAddress &address);
    void receiveMessage(const QString &from, const QString &msg);
    void receivePoint(const QString &from, XYQiziWidget *qizi);


public slots:
    void receiveData(const QString &from, const QByteArray &data, int type);
    void sendMessage(const QHostAddress &address, const QString &msg);
    void sendQizi(const QHostAddress &address, XYQiziWidget *qizi);

protected:
    void timerEvent(QTimerEvent *event);

private:
    XYUdpbroadcast *socket;

};

#endif // XYQISHOU_H
