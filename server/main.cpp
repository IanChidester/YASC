#include "serverwindow.h"
#include <QApplication>
#include "yascserver.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ServerWindow w;
    w.show();

    YASCServer server;
    bool connectionOpened = server.startServer();
    if (connectionOpened) {
        qDebug() << "connection opened";
    }
    else {
        qDebug() << "connection not opened";
    }

    w.setIP(server.ipAddress);
    w.setPort(server.port);

    return a.exec();
}
