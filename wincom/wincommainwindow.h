#ifndef WINCOMMAINWINDOW_H
#define WINCOMMAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include "win_qextserialport.h"
namespace Ui {
class WincomMainWindow;
}

class WincomMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit WincomMainWindow(QWidget *parent = 0);
    ~WincomMainWindow();

private:
    Ui::WincomMainWindow *ui;
    Win_QextSerialPort *myCom;  //声明对象
private slots:
    void readMyCom();//声明对象 槽函数
};

#endif // WINCOMMAINWINDOW_H
