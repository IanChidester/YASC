#ifndef YASCSERVER_H
#define YASCSERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QMap>
#include <QDebug>
#include <QList>

class YASCServer : public QObject
{
    Q_OBJECT

private:
    QTcpServer *server;
    QMap<QTcpSocket*, QString> clients;
    void sendToAllBut(QTcpSocket*, QByteArray&);

public:
    explicit YASCServer(QObject *parent = 0);
    bool startServer();
    ~YASCServer();
    const static quint16 port = 12345;
    QString ipAddress;

signals:
    void clientDisconnected();

public slots:
    void newConnection();
    void readyRead();
    void socketDisconnected();
    void acceptError(QAbstractSocket::SocketError socketError);
};

#endif // YASCSERVER_H
