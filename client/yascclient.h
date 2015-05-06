#ifndef YASCCLIENT_H
#define YASCCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include <QHostAddress>

class YASCClient : public QObject
{
    Q_OBJECT
private:
    QTcpSocket* socket;
    QString nickname;

public:
    explicit YASCClient(QObject *parent = 0);
    ~YASCClient();
    bool run(QString ip, QString port, QString nickname);
    QString getNickname();
    //void setNickname(); dont think we need this

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
