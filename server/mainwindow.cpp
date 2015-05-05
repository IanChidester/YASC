#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setIP(QString ip) {
    ui->lineEdit->setText(ip);
}

void MainWindow::setPort(qint16 port) {
    ui->lineEdit_2->setText(QString::number(port));
}
