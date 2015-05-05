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
    ~YASCServer();
    const static quint16 port = 12345;
    QString ipAddress;

signals:

public slots:
    void newConnection();
    void readyRead();
    void disconnected();
};

#endif // YASCSERVER_H
