#include "yascclient.h"

YASCClient::YASCClient(QObject *parent) : QObject(parent)
{
    crypto.setKey(0xf792525d4e470448); //set the encryptoin key
}

YASCClient::~YASCClient()
{
    socket->disconnectFromHost();
}

bool YASCClient::run(QString ip, QString port, QString nickname) {
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    qDebug() << "connecting from client";

    this->nickname = nickname;

    socket->connectToHost(QHostAddress(ip), port.toInt());

    if(!socket->waitForConnected(5000)) {
        qDebug() << "Error: " << socket->errorString();
        return false;
    }
    return true;
}

void YASCClient::connected() {
    qDebug() << "connected...";

    QByteArray nick = crypto.encryptToByteArray(nickname);
    socket->write(nick);
}

void YASCClient::disconnected() {
    qDebug() << "disconnected...";
}

void YASCClient::bytesWritten(qint64 bytes) {
    qDebug() << bytes << " bytes written...";
}

void YASCClient::readyRead() {
    QByteArray message = crypto.decryptToByteArray(socket->readAll());
    emit incomingMessage(QString(message));
    qDebug() << QString(message);
}

void YASCClient::sendMessage(QString message) {
    qDebug() << "sending message...";

    QByteArray msgToSend = crypto.encryptToByteArray(message);
    socket->write(msgToSend);
}

QString YASCClient::getNickname() {
    return this->nickname;
}
