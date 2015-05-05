#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

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

void MainWindow::setClient(YASCClient* client)
{
    this->client = client;
    connect(client, SIGNAL(incomingMessage(QString)), this, SLOT(onNewMessage(QString)));
}

void MainWindow::on_inputEdit_returnPressed()
{
    client->sendMessage(ui->inputEdit->text());
    ui->outputText->append(client->getNickname() + ": " + ui->inputEdit->text());
    ui->inputEdit->clear();
}

void MainWindow::on_sendButton_clicked()
{
    on_inputEdit_returnPressed();
}

void MainWindow::onNewMessage(QString message)
{
    ui->outputText->append(message);
}
