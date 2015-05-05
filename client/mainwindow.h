#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "yascclient.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setClient(YASCClient* client);

private slots:
    void on_inputEdit_returnPressed();
    void on_sendButton_clicked();
    void onNewMessage(QString);

private:
    Ui::MainWindow *ui;
    YASCClient* client;
};

#endif // MAINWINDOW_H
