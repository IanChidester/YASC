#ifndef YASCCLIENT_H
#define YASCCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include <QHostAddress>
#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include <QHostAddress>
#include <QIODevice>
#include <QAudioInput>
#include <QObject>
#include <QByteArray>
#include <QDebug>
#include <QString>
#include <QAudioFormat>
#include <QAudioOutput>
#include "simplecrypt.h"

class YASCClient : public QObject
{
    Q_OBJECT
private:
    bool setupAudioSocket(QString ip, QString port);
    void setupAudioInputOutput();

    QTcpSocket* socket;
    QTcpSocket* audioSocket;
    QString nickname;

    QAudioFormat getFormat();

    QIODevice *m_output;
    QAudioOutput *m_audioOutput;
    QIODevice *m_input;
    QAudioInput *m_audioInput;
    QAudioDeviceInfo m_device;
    QByteArray m_buffer;

    const int BufferSize = 4096;
    SimpleCrypt crypto;

public:
    explicit YASCClient(QObject *parent = 0);
    ~YASCClient();
    bool run(QString ip, QString port, QString nickname);
    bool audioRun(QString ip, QString port = "12346");
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
    void incommingAudioReadyRead();
    void audioReadMore();
};

#endif // YASCCLIENT_H
