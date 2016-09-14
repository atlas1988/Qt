#include "wincommainwindow.h"
#include "ui_wincommainwindow.h"

WincomMainWindow::WincomMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WincomMainWindow)
{
    ui->setupUi(this);
    // 定义一个结构体，用来存放串口各个参数
    struct PortSettings myComSetting = {BAUD115200,DATA_8,PAR_NONE,STOP_1,FLOW_OFF,500};
   //   定义串口对象，并传递参数在构造函数对其进行初始化
    myCom = new Win_QextSerialPort("COM4",myComSetting,QextSerialBase::EventDriven);
  //    以读写的方式打开串口
    myCom ->open(QIODevice::ReadWrite);

   //    信号跟槽函数关联，当串口缓冲区有数据时，进行串口操作
    connect(myCom,SIGNAL(readyRead()),this,SLOT(readMyCom()));


}
//   读串口函数
void WincomMainWindow::readMyCom()
{
    //  读取串口缓冲区的所有数据给临时变量 tmp
    QByteArray temp = myCom->readAll();
    //  将串口的数据显示在窗口的文本浏览器中
    ui->textBrowser->insertPlainText(temp);
}
WincomMainWindow::~WincomMainWindow()
{
    delete ui;
}
