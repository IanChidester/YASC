#include "mainwindow.h"
#include <QApplication>
#include "yascserver.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    YASCServer server;



    w.setIP(server.ipAddress);
    w.setPort(server.port);
    qDebug() << server.port;

    return a.exec();
}
