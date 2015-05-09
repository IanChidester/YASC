#include "yascserver.h"

YASCServer::YASCServer(QObject *parent) : QObject(parent) {
    server = new QTcpServer(this);
    audioServer = new QTcpServer(this);
    crypto.setKey(0xf792525d4e470448); //set the shared encryption key
}

YASCServer::~YASCServer() {
    server->close();
    delete server;

    audioServer->close();
    delete audioServer;
}

bool YASCServer::startServer() {
    qDebug() << "Starting server.";

    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    connect(server, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(acceptError(QAbstractSocket::SocketError)));

    if(server->listen(QHostAddress::Any, port) == false) {
        qDebug() << "Server could not start.";
        return false;
    }

    qDebug() << "Starting audio server.";

    connect(audioServer, SIGNAL(newConnection()), this, SLOT(newAudioConnection()));

    if(audioServer->listen(QHostAddress::Any, audioPort) == false) {
        qDebug() << "Audio server could not start.";
        // return false;
    }


    qDebug() << "Server started!";
    return true;
}

void YASCServer::newConnection() {
    qDebug() << "New incoming connection";

    QTcpSocket* socket = server->nextPendingConnection();

    qDebug() << clients.count() << " clients.";

    QString strMotd = "MOTD\n" + QString::number(clients.count()) + " clients connected."; //set the motd
    QByteArray motd = crypto.encryptToByteArray(strMotd); //encrypt the str
    socket->write(motd);

    socket->flush();

    connect(socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

}

void YASCServer::readyRead() {
    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());

    auto data = socket->readAll();
    QByteArray dataDecrypted = crypto.decryptToByteArray(data);

    qDebug() << "incomming data from: " << socket->peerAddress().toString();
    qDebug() << dataDecrypted;

    if (!clients.contains(socket)) {
        //new user
        auto username = dataDecrypted.left(10);
        clients[socket] = username;
        dataDecrypted = " has connected.";
    }

    sendToAllBut(socket, dataDecrypted);
}

void YASCServer::socketDisconnected() {
    emit clientDisconnected();

    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());

    qDebug() << socket->peerAddress().toString() << " closed the connection.";

    auto username = clients.value(socket);
    if (clients.remove(socket) > 0) {
        QByteArray data;
        data = data.append(username).append(" has disconnected.");

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
    message = crypto.encryptToByteArray(message);
    foreach(QTcpSocket* client, clients.keys())
        if (client != socket)
            client->write(message);
}

void YASCServer::acceptError(QAbstractSocket::SocketError socketError)
{
    qDebug()<<socketError;
}

void YASCServer::newAudioConnection() {
    qDebug() << "New incoming audio connection";

    QTcpSocket *socket = audioServer->nextPendingConnection();

    audioClients.append(socket);

    qDebug() << audioClients.count() << " clients.";

    connect(socket, SIGNAL(disconnected()), this, SLOT(audioDisconnection()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(audioReadyRead()));
}

void YASCServer::audioReadyRead() {
    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());

    auto data = socket->readAll();

    qDebug() << "incomming audio data from: " << socket->peerAddress().toString();

    foreach(QTcpSocket* client, audioClients)
        if(client != socket)
            client->write(data);
}

void YASCServer::audioDisconnection() {
    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());

    qDebug() << socket->peerAddress().toString() << " closed the audio connection.";

    if (audioClients.removeOne(socket) > 0) {
        QByteArray data;
        data.append(" has disconnected audio.");

        sendToAllBut(socket, data);
    }

    qDebug() << audioClients.count() << " audio clients.";
}
