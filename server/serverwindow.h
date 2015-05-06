#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class ServerWindow;
}

class ServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ServerWindow(QWidget *parent = 0);
    ~ServerWindow();
    void setIP(QString ip);
    void setPort(qint16 port);

private:
    Ui::ServerWindow *ui;
};

#endif // MAINWINDOW_H
