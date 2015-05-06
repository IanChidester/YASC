#include "yascclient.h"

YASCClient::YASCClient(QObject *parent) : QObject(parent)
{

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
    qDebug() << "client connected";

    socket->write(nickname.toStdString().c_str());
    socket->flush();
}

void YASCClient::disconnected() {
    qDebug() << "client disconnected";
}

void YASCClient::bytesWritten(qint64 bytes) {
    qDebug() << bytes << " bytes written from client";
}

void YASCClient::readyRead() {
    emit incomingMessage(socket->readAll());
}

void YASCClient::sendMessage(QString message) {
    qDebug() << "sending message from client";

    socket->write(message.toStdString().c_str());
}

QString YASCClient::getNickname() {
    return this->nickname;
}
