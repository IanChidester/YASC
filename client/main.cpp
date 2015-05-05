#include "yascclient.h"
#include "initialwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    YASCClient c;
    initialWindow w;
    w.show();

    return a.exec();
}
