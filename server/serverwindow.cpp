#include "serverwindow.h"
#include "ui_serverwindow.h"

ServerWindow::ServerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ServerWindow)
{
    ui->setupUi(this);
}

ServerWindow::~ServerWindow()
{
    delete ui;
}

void ServerWindow::setIP(QString ip) {
    ui->lineEdit->setText(ip);
}

void ServerWindow::setPort(qint16 port) {
    ui->lineEdit_2->setText(QString::number(port));
}
