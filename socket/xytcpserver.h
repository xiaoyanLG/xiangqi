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
    bool isSender();
    bool isConnected();
    void abort();

signals:
    void receiveUserData(const QString &from, const QByteArray &data, int type, bool udp = false);
    void stateChanged(QAbstractSocket::SocketState state);

public slots:
    void connectToHost(const QHostAddress &address);
    void writeUserData(const QByteArray &data, int type);

protected slots:
    void receiveData();

protected:
    void incomingConnection(int socketDescriptor);

private:
    QTcpSocket *client;
    QString     userName;
    int         port;
    bool        sender;
};

#endif // XYTCPSOCKET_H
