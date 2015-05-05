#include "yascserver.h"

YASCServer::YASCServer(QObject *parent) : QObject(parent) {
    server = new QTcpServer(this);

    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));

    if(server->listen(QHostAddress::Any, port)) {
        qDebug() << "Server started!";
    } else {
        qDebug() << "Server could not start.";
    }

    //set the local address
    QTcpSocket socket;
    socket.connectToHost("8.8.8.8", 53); // google DNS, or something else reliable
    if (socket.waitForConnected()) {
        qDebug()
            << "local IPv4 address for Internet connectivity is"
            << socket.localAddress();
        this->ipAddress = socket.localAddress().toString();
    } else {
        qWarning()
            << "could not determine local IPv4 address:"
            << socket.errorString();
        this->ipAddress = "127.0.0.1";
    }
}

YASCServer::~YASCServer() {

}

void YASCServer::newConnection() {
    QTcpSocket* socket = server->nextPendingConnection();

    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    qDebug() << clients.count() << " clients.";

    QString motd = "MOTD\n" + QString::number(clients.count()) + " clients connected.";
    socket->write(motd.toStdString().c_str());

//    socket->write("Hello client\r\n");
    socket->flush();
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

void YASCServer::disconnected() {
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
