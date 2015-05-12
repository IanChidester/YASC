#ifndef YASCSERVER_H
#define YASCSERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QMap>
#include <QDebug>
#include <QList>
#include "simplecrypt.h"

class YASCServer : public QObject
{
    Q_OBJECT

private:
    QTcpServer *server;
    QTcpServer *audioServer;
    QMap<QTcpSocket*, QString> clients;
    QList<QTcpSocket*> audioClients;
    void sendToAllBut(QTcpSocket*, QByteArray&);
    SimpleCrypt crypto;

public:
    explicit YASCServer(QObject *parent = 0);
    bool startServer();
    ~YASCServer();
    const static quint16 port = 12345;
    const static quint16 audioPort = 12346;
    QString ipAddress;

signals:
    void clientDisconnected();
    void serverReady();

public slots:
    void newAudioConnection();
    void audioReadyRead();
    void audioDisconnection();
    void newConnection();
    void readyRead();
    void socketDisconnected();
    void acceptError(QAbstractSocket::SocketError socketError);
};

#endif // YASCSERVER_H
