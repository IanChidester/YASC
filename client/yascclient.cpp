#include "yascclient.h"

YASCClient::YASCClient(QObject *parent) : QObject(parent)
{

}

YASCClient::~YASCClient()
{

}

void YASCClient::run(QString ip, QString port, QString nickname) {
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    qDebug() << "connecting...";

    this->nickname = nickname;

    socket->connectToHost(QHostAddress(ip), port.toInt());

    if(!socket->waitForConnected(5000)) {
        qDebug() << "Error: " << socket->errorString();
    }
}

void YASCClient::connected() {
    qDebug() << "connected...";

    socket->write(nickname.toStdString().c_str());
}

void YASCClient::disconnected() {
    qDebug() << "disconnected...";
}

void YASCClient::bytesWritten(qint64 bytes) {
    qDebug() << bytes << " bytes written...";
}

void YASCClient::readyRead() {
    emit incomingMessage(socket->readAll());
}

void YASCClient::sendMessage(QString message) {
    qDebug() << "sending message...";

    socket->write(message.toStdString().c_str());
}

QString YASCClient::getNickname() {
    return this->nickname;
}
