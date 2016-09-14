#include "wincommainwindow.h"
#include "ui_wincommainwindow.h"

WincomMainWindow::WincomMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WincomMainWindow)
{
    ui->setupUi(this);
    ui->closeMyComBtn->setEnabled(false); // 开始“关闭串口”按钮不可用
    ui->sendMsgBtn->setEnabled(false);    // 开始“发送数据”按钮不可用

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

void WincomMainWindow::on_openMyComBtn_clicked()
{
    // 定义一个结构体，用来存放串口各个参数
    struct PortSettings myComSetting = {BAUD115200,DATA_8,PAR_NONE,STOP_1,FLOW_OFF,500};
   //   定义串口对象，并传递参数在构造函数对其进行初始化
    myCom = new Win_QextSerialPort("COM4",myComSetting,QextSerialBase::EventDriven);
  //    以读写的方式打开串口
 //   myCom ->open(QIODevice::ReadWrite);
    if(myCom->open(QIODevice::ReadWrite))    //打开串口
      {
          myCom->setBaudRate(BAUD115200);   //设置波特率
          myCom->setDataBits(DATA_8);     //设置数据位
          myCom->setStopBits(STOP_1);     //设置停止位
          myCom->setParity(PAR_NONE);     //设置校验位
          myCom->setFlowControl(FLOW_OFF);//设置流控制
          myCom->setTimeout(500);         //设置超时时间
      }
   //    信号跟槽函数关联，当串口缓冲区有数据时，进行串口操作
    connect(myCom,SIGNAL(readyRead()),this,SLOT(readMyCom()));
    ui->openMyComBtn->setEnabled(false); // 打开串口后 “打开串口“按钮不可用
    ui->closeMyComBtn->setEnabled(true); // 打开串口后  "关闭串口"按钮可用
    ui->sendMsgBtn->setEnabled(true);   // 打开串口后 “发送数据”按钮可用
}

void WincomMainWindow::on_closeMyComBtn_clicked()
{
    myCom->close(); // 关闭串口， 改函数在 win_qextserialport.cpp文件中定义
    ui->openMyComBtn->setEnabled(true); //关闭串口后 “打开串口”按钮可用
    ui->closeMyComBtn->setEnabled(false); // 关闭串口后“关闭串口”按钮不可用
    ui->sendMsgBtn->setEnabled(false); // 关闭串口后“发送数据”按钮不可用
}

void WincomMainWindow::on_sendMsgBtn_clicked()
{
    //将数据写入串口
    myCom->write(ui->sendMsgLineEdit->text().toLatin1());
}
