#ifndef YASCCLIENT_H
#define YASCCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include <QHostAddress>
#include <thread>
#include "simplecrypt.h"

class YASCClient : public QObject
{
    Q_OBJECT
private:
    QTcpSocket* socket;
    QString nickname;
    SimpleCrypt crypto;
    std::thread adThread;

public:
    explicit YASCClient(QObject *parent = 0);
    ~YASCClient();
    bool run(QString ip, QString port, QString nickname);
    QString getNickname();
    //void setNickname(); dont think we need this

private:
    void adThreadRun();

signals:
    void incomingMessage(QString message);

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();
    void sendMessage(QString mesage);
};

#endif // YASCCLIENT_H
