#include "initialwindow.h"
#include "mainwindow.h"
#include "ui_initialwindow.h"

initialWindow::initialWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::initialWindow)
{
    ui->setupUi(this);
}

initialWindow::~initialWindow()
{
    delete ui;
}

void initialWindow::on_pushButton_clicked()
{
    client = new YASCClient(this);
    client->run(ui->lineEdit->text(), ui->lineEdit_2->text(), ui->lineEdit_3->text());
    client->audioRun(ui->lineEdit->text());
    MainWindow* mainWindow = new MainWindow();
    mainWindow->setClient(client);
    mainWindow->show();
    this->close();
}
