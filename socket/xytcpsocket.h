#ifndef XYTCPSOCKET_H
#define XYTCPSOCKET_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class XYTcpSocket : public QObject
{
    Q_OBJECT
public:
    explicit XYTcpSocket(QObject *parent = 0);

public slots:
    void connectToHost(const QHostAddress &address);
    void newConnect();
    void receiveData();

private:
    QTcpServer *server;
    QTcpSocket *client;
    int         port;
};

#endif // XYTCPSOCKET_H
