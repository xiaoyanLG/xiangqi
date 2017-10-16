#ifndef XYTCPSOCKET_H
#define XYTCPSOCKET_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class XYTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit XYTcpServer(QObject *parent = 0);

signals:
    void receiveUserData(const QByteArray &data, int type);

public slots:
    void connectToHost(const QHostAddress &address);
    void stateChanged(QAbstractSocket::SocketState state);
    void writeUserData(const QByteArray &data, int type);

protected slots:
    void receiveData();

protected:
    void incomingConnection(int socketDescriptor);

private:
    QTcpSocket *client;
    int         port;
};

#endif // XYTCPSOCKET_H
