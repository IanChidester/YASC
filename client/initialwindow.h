#ifndef INITIALWINDOW_H
#define INITIALWINDOW_H

#include "yascclient.h"
#include <QMainWindow>

namespace Ui {
class initialWindow;
}

class initialWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit initialWindow(QWidget *parent = 0);
    ~initialWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::initialWindow *ui;
    YASCClient* client;
};

#endif // INITIALWINDOW_H
