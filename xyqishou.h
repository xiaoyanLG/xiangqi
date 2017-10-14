#ifndef XYQISHOU_H
#define XYQISHOU_H

#include <QObject>
#include "xyudpbroadcast.h"

class XYQishou : public QObject
{
    Q_OBJECT
public:
    explicit XYQishou(QObject *parent = 0);
    ~XYQishou();

signals:
    void peopleUpline(const QString &name, const QHostAddress &address);
    void peopleOffline(const QString &name, const QHostAddress &address);
    void receiveData(const QString &from, const QString &data);

public slots:
    void sendMessage(const QHostAddress &address, const QString &msg);

protected:
    void timerEvent(QTimerEvent *event);

private:
    XYUdpbroadcast *socket;

};

#endif // XYQISHOU_H
