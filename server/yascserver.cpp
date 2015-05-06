#include "yascserver.h"

YASCServer::YASCServer(QObject *parent) : QObject(parent) {
    server = new QTcpServer(this);
}

YASCServer::~YASCServer() {
    server->close();
    delete server;
}

bool YASCServer::startServer() {
    qDebug() << "Starting server.";

    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    connect(server, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(acceptError(QAbstractSocket::SocketError)));

    if(server->listen(QHostAddress::Any, port) == false) {
        qDebug() << "Server could not start.";
        return false;
    }

    //set the local address
//    QTcpSocket socket;
//    socket.connectToHost("216.58.217.206", 80);
//    if (socket.waitForConnected(5000)) {
//        this->ipAddress = socket.localAddress().toString();
//    } else {
//        this->ipAddress = "127.0.0.1";
//        qDebug() << "Couldn't find own IP";
//        return false;
//    }
    qDebug() << "Server started!";
    return true;
}

void YASCServer::newConnection() {
    qDebug() << "New incoming connection";

    QTcpSocket* socket = server->nextPendingConnection();

    qDebug() << clients.count() << " clients.";

    QString motd = "MOTD\n" + QString::number(clients.count()) + " clients connected.";
    socket->write(motd.toStdString().c_str());

//    socket->write("Hello client\r\n");
    socket->flush();

    connect(socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

}

void YASCServer::readyRead() {
    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());

    auto data = socket->readAll();

    qDebug() << "incomming data from: " << socket->peerAddress().toString();
    qDebug() << data;

    if (!clients.contains(socket)) {
        //new user
        auto username = data.left(10);
        clients[socket] = username;
        data = " has connected.";
    }

    sendToAllBut(socket, data);
}

void YASCServer::socketDisconnected() {
    emit clientDisconnected();

    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());

    qDebug() << socket->peerAddress().toString() << " closed the connection.";

    if (clients.remove(socket) > 0) {
        QByteArray data;
        data.append(" has disconnected.");

        sendToAllBut(socket, data);
    }

    qDebug() << clients.count() << " clients.";
}

void YASCServer::sendToAllBut(QTcpSocket* socket, QByteArray& data) {
    QString name = socket->peerName();

    if (clients.contains(socket)) {
        name = clients.find(socket).value();
    }

    QByteArray message = QByteArray().append(name).append(": ").append(data);

    foreach(QTcpSocket* client, clients.keys())
        if (client != socket)
            client->write(message);
}

void YASCServer::acceptError(QAbstractSocket::SocketError socketError)
{
    qDebug()<<socketError;
}
