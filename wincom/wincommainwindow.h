#ifndef WINCOMMAINWINDOW_H
#define WINCOMMAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include "win_qextserialport.h"
#include "aboutdialog.h"
#include <QMessageBox>
#include <QFile>
#include <QInputDialog>
#include <QFileDialog>
#include <QTextStream>

// 延时 TIME_OUT串口读写延时
#define TIME_OUT        10
// 读取定时器计时间隔 200ms 读取定时器是我们读取串口缓存的延时
#define TIMER_INTERVAL   200
//连续发送定时计时间隔 200ms
#define OBO_TIMER_INTERVAL  200
// 载入文件时如果文件大小超过TIP_FILE_SIZE字节提示文件过大是否继续打开
#define TIP_FILE_SIZE   5000
//载入文件最大长度限制在 MAX_FILE_SIZE 字节内
#define MAX_FILE_SIZE   10000

namespace Ui {
class WincomMainWindow;
}

class WincomMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit WincomMainWindow(QWidget *parent = 0);
    ~WincomMainWindow();
    // 初始化函数
    void startInit();

protected:
    void changeEvent(QEvent *e);
    void setActionsEnable(bool status); // 设置操作下拉菜单可用不可用
    void setComboxEnable(bool status); //设置下拉选项菜单可用不可用
private:
    Ui::WincomMainWindow *ui;
    AboutDialog aboutdlg; // 声明关于对话框
    Win_QextSerialPort *myCom;  //声明对象
    QTimer *timer;              // 声明延时定时器
    QTimer *obotimer;           //声明连续发送间隔计时器
    unsigned int timerdly;
    unsigned int obotimerdly;
    QString write2fileName;     // 保存读取到的串口数据到该文件
private slots:
    void readMyCom();//声明对象 槽函数
    void sendMsg();// 声明对象 发送数据
 //   void on_openMyComBtn_clicked();
//  void on_closeMyComBtn_clicked();
    void on_sendMsgBtn_clicked();
    void on_clearDisplayBtn_clicked();
    void on_actionAdd_triggered();
    void on_actionOpen_triggered();
    void on_actionClose_triggered();
    void on_actionAbout_triggered();
    void on_actionSave_triggered();
    void on_actionReadFile_triggered();
    void on_actionWriteFile_triggered();
    void on_actionClearCount_triggered();
    void on_actionClearCom_triggered();
    void on_actionExit_triggered();
    void on_continueSendCheckBox_clicked();
    void on_intervalSpinBox_valueChanged(int arg1);
    void on_sendHexCheckBox_clicked();

};

#endif // WINCOMMAINWINDOW_H
