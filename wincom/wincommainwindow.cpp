#include "wincommainwindow.h"
#include "ui_wincommainwindow.h"

WincomMainWindow::WincomMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WincomMainWindow)
{
    ui->setupUi(this);
    startInit(); //初始化参数
    myCom = NULL;
 //左下角显示状态
    ui->statusBar->showMessage(tr("欢迎使用串口工具！"));

}

WincomMainWindow::~WincomMainWindow()
{
    if(myCom != NULL){
        if(myCom->isOpen()){
            myCom->close();
        }
        delete myCom;
    }
    delete ui;
}

//change event一般是当前widget状态改变后触发的
//如字体改变、语言改变之类的。
//该方法主要捕获改变事件，当语言改变后，执行相关操作。
void WincomMainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch(e->type()){
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

// 初始化
void WincomMainWindow::startInit()
{
    setActionsEnable(false); // 关闭菜单使其不可用
    ui->intervalSpinBox->setEnabled(false); //使间隔不能用
    ui->continueSendCheckBox->setEnabled(false); // 使连续发送不可用
    ui->sendMsgBtn->setEnabled(false);
    ui->sendMsgLineEdit->setEnabled(false); // 使发送消息窗口不可用
    ui->actionAdd->setEnabled(true); //使添加端口可以使用

    //初始化读取定时器计时间隔
    timerdly = TIMER_INTERVAL;
    //初始化连续发送计时器时间间隔
    obotimerdly = OBO_TIMER_INTERVAL;
    //设置读取计时器
    //timer = new QTimer(this);
    //connect(timer,SIGNAL(timeout()),this,SLOT(readMyCom()));
    //设置连续发送计时器
    obotimer = new QTimer(this);
    connect(obotimer,SIGNAL(timeout()),this,SLOT(sendMsg()));


}
void WincomMainWindow::setActionsEnable(bool status)
{
    ui->actionSave->setEnabled(status);// 设置“保存”菜单 使能
    ui->actionClose->setEnabled(status);// 设置“关闭”菜单 使能
    ui->actionReadFile->setEnabled(status);// 设置”读取文件“菜单 使能
    ui->actionClearCom->setEnabled(status);// 设置“清除串口”菜单 使能
    ui->actionWriteFile->setEnabled(status);// 设置“写入文件”菜单 使能
}
void WincomMainWindow::setComboxEnable(bool status)
{
    ui->portNameComboBox->setEditable(status); // 设置“端口名” 使能
    ui->baudRateComboBox->setEnabled(status); //  设置“波特率” 使能
    ui->dataBitsComboBox->setEnabled(status); //  设置“数据位” 使能
    ui->parityComboBox->setEnabled(status); // 设置“奇偶校验位” 使能
    ui->stopBitsComboBox->setEnabled(status); // 设置“停止位” 使能
}
//   读串口函数
void WincomMainWindow::readMyCom()
{
    //  读取串口缓冲区的所有数据给临时变量 tmp
    QByteArray recvTemp = myCom->readAll();
    QString buf;
    if(!recvTemp.isEmpty()){// 串口有数据
        // 判断写入文件名是否有定义
        ui->textBrowser->setTextColor(Qt::black);
        if(ui->displayCharRBtn->isChecked()){// 字符串形式显示
                buf = recvTemp;
        }else if(ui->displayHexRBtn->isChecked()){// 十六进制显示HEX
            QString str;
            for(int i=0;i<recvTemp.count();i++){
                QString s;
                s.sprintf("0x%02x,",(unsigned char)recvTemp.at(i));
                buf +=s;
            }
          }else{//
                buf = recvTemp;
            }

        if(!write2fileName.isEmpty()){
            QFile file(write2fileName);
             // 如果打开失败则返回给出提示并退出函数
            if(!file.open(QFile::Append | QIODevice::Text)){
                QMessageBox::warning(this,tr("写入文件"),tr("打开文件%1失败，无法写入\n%2").arg(write2fileName).arg(file.errorString()),QMessageBox::Ok);
                return;
            }
            QTextStream out(&file);
            out<<buf;
            file.close();
            //左下角显示状态
            ui->statusBar->showMessage(tr("接收:数据已经写入文件 %1").arg(write2fileName));
        }
    //ui->textBrowser->setText(ui->textBrowser->document()->toPlainText()+buf);
    ui->textBrowser->insertPlainText(buf);
    QTextCursor cursor = ui->textBrowser->textCursor(); // 获取显示窗口的光标位置
    cursor.movePosition(QTextCursor::End);//将光标移到到末尾
    ui->textBrowser->setTextCursor(cursor);
    // 显示读取到的数据
    ui->readLcdNumber->display(ui->readLcdNumber->value() + recvTemp.size());
    ui->statusBar->showMessage(tr("成功读取%1字节数据").arg(recvTemp.size()));

    }
}
// 发送数据
void WincomMainWindow::sendMsg()
{
    QByteArray sendBuf;
    if(ui->sendHexCheckBox->isChecked()){
        QString str;
        bool ok;
        char data;
        QStringList list;
        str = ui->sendMsgLineEdit->text();
        list = str.split(" ");
        for(int i=0;i<list.count();i++){
            if(list.at(i) == " ")
                continue;
            if(list.at(i).isEmpty())
                continue;
            data = (char)list.at(i).toInt(&ok,16);
            if(!ok){
                QMessageBox::information(this, tr("提示消息"), tr("输入数据格式有误"), QMessageBox::Ok);
                if(obotimer != NULL)
                    obotimer->stop();
                ui->sendMsgBtn->setText(tr("发 送"));
              //  ui->sendmsgBtn->setIcon(QIcon(":new/prefix1/src/send.png"));
                return;
            }
            sendBuf.append(data);
        }
    }

    sendBuf = ui->sendMsgLineEdit->text().toLocal8Bit();
    // 发送数据
    myCom->write(sendBuf);
    ui->statusBar->showMessage(tr("发送数据成功"));
    //ui->textBrowser->append(tr("发送：") + ui->sendMsgLineEdit->text());
    // 界面控制
    ui->textBrowser->setTextColor(Qt::lightGray);
}

void WincomMainWindow::on_sendMsgBtn_clicked()
{

    //如果当前正在连续发送数据，暂停发送
    if(ui->sendMsgBtn->text() == tr("暂停")){
        obotimer->stop();
        ui->sendMsgBtn->setText(tr("发送"));
        //ui->sendMsgBtn->setIcon(QIcon(":new/prefix1/src/send.png"));
        return;
    }
    // 如果发送数据为空，给出提示返回
    if(ui->sendMsgLineEdit->text().isEmpty()){
        QMessageBox::information(this,tr("提示消息"),tr("没有需要发送的数据"),QMessageBox::Ok);
        return;
    }
     //如果不是连续发送
    if(!ui->continueSendCheckBox->isChecked()){
        ui->sendMsgLineEdit->setFocus();
        // 发送数据
        sendMsg();
    }else{//连续发送
        obotimer->start(obotimerdly);
        ui->sendMsgBtn->setText(tr("暂停"));
        //ui->sendMsgBtn->setIcon(QIcon(:new/prefix1/src/pause.png));
    }
}
//清空显示记录
void WincomMainWindow::on_clearDisplayBtn_clicked()
{
    ui->textBrowser->clear();
    ui->statusBar->showMessage(tr("记录已经清空"));
}
// 添加串口
void WincomMainWindow::on_actionAdd_triggered()
{
    bool ok = false;
    QString portName;
    portName = QInputDialog::getText(this,tr("添加串口"),tr("设备名称"),QLineEdit::Normal,0,&ok);
    if(ok && !portName.isEmpty()){
        ui->portNameComboBox->addItem(portName);
        ui->statusBar->showMessage(tr("添加串口成功"));
    }
}
// 打开串口
void WincomMainWindow::on_actionOpen_triggered()
{
    QString portName = ui->portNameComboBox->currentText();// 获取串口名
    // 这里使用QextSerialBase::Polling （QextSerialBase::QueryMode） 轮询的模式 // EventDriven
   // struct PortSettings myComSetting = {BAUD9600,DATA_8,PAR_NONE,STOP_1,FLOW_OFF, 500};
    myCom =new Win_QextSerialPort(portName,QextSerialBase::EventDriven);
    //    信号跟槽函数关联，当串口缓冲区有数据时，进行串口操作
    connect(myCom,SIGNAL(readyRead()),this,SLOT(readMyCom()));
    if(myCom->open(QIODevice::ReadWrite)){// 以读写的方式打开串口
        QMessageBox::information(this, tr("打开成功"), tr("已成功打开串口 ") + portName, QMessageBox::Ok);
      }else{
        QMessageBox::critical(this, tr("打开失败"), tr("串口打开失败 ") + portName + tr("\n该串口设备不存在或已被占用"), QMessageBox::Ok);
        return;
    }
/*
    // 设置波特率
    myCom->setBaudRate((BaudRateType)ui->baudRateComboBox->currentIndex());
    // 设置数据位
    myCom->setDataBits((DataBitsType)ui->dataBitsComboBox->currentIndex());
    //设置校验
    myCom->setParity((ParityType)ui->parityComboBox->currentIndex());
    //设置停止位
    myCom->setStopBits((StopBitsType)ui->stopBitsComboBox->currentIndex());
*/  // 根据组合框内容对串口进行设置
    // 设置波特率
    if(ui->baudRateComboBox->currentText() == tr("9600"))
        myCom->setBaudRate(BAUD9600);
    else if(ui->baudRateComboBox->currentText() == tr("115200"))
        myCom->setBaudRate(BAUD115200);
    // 设置数据位
     if(ui->dataBitsComboBox->currentText() == tr("无"))
        myCom->setParity(PAR_NONE);
     else if(ui->dataBitsComboBox->currentText() == tr("奇"))
        myCom->setParity(PAR_ODD);
     else if(ui->parityComboBox->currentText() == tr("偶"))
         myCom->setParity(PAR_EVEN);
//     myCom->setParity(PAR_NONE);
     // 设置停止位
     if(ui->stopBitsComboBox->currentText() == tr("1"))
         myCom->setStopBits(STOP_1);
     else if(ui->stopBitsComboBox->currentText() == tr("2"))
         myCom->setStopBits(STOP_2);
    //设置数据流控制
    myCom->setFlowControl(FLOW_OFF);
    //设置延时
    myCom->setTimeout(TIME_OUT);
    //界面控制
    ui->sendMsgBtn->setEnabled(true);
    ui->sendMsgLineEdit->setEnabled(true);
    ui->actionOpen->setEnabled(false);
    ui->sendMsgLineEdit->setFocus();
    ui->continueSendCheckBox->setEnabled(true);
    ui->actionAdd->setEnabled(false);
    setComboxEnable(false);
    setActionsEnable(true);
    //开启读取定时器
   // timer->start(timerdly);


    ui->statusBar->showMessage(tr("打开串口成功"));
}
// 关闭串口
void WincomMainWindow::on_actionClose_triggered()
{
    myCom->close();
    delete myCom;
    timer->stop();
    ui->sendMsgBtn->setEnabled(false);
    setComboxEnable(true);
    ui->actionOpen->setEnabled(true);
    ui->sendMsgLineEdit->setEnabled(false);
    ui->continueSendCheckBox->setEnabled(false);
    ui->actionAdd->setEnabled(true);
    setActionsEnable(false);
    ui->actionWriteFile->setChecked(false);
    ui->statusBar->showMessage(tr("串口已经关闭"));

}
// 关于
void WincomMainWindow::on_actionAbout_triggered()
{
    aboutdlg.show();
    ui->statusBar->showMessage(tr("关于Wincom"));
}
// 保存textBrowser中的内容
void WincomMainWindow::on_actionSave_triggered()
{
    if(ui->textBrowser->toPlainText().isEmpty()){
        QMessageBox::information(this,tr("提示消息"),tr("还没有数据!"), QMessageBox::Ok);
        return;
    }
    QString filename = QFileDialog::getSaveFileName(this,tr("保存为"),tr("未命名.txt"));
    QFile file(filename);
    //如果用户取消了保存则直接退出函数
    if(file.fileName().isEmpty()){
        return;
    }
    //如果打开失败则给出提示并退出函数
    if(!file.open(QFile::WriteOnly | QIODevice::Text)){
        QMessageBox::warning(this, tr("保存文件"), tr("打开文件 %1 失败, 无法保存\n%2").arg(filename).arg(file.errorString()), QMessageBox::Ok);
        return;
    }
    // 写数据到文件
    QTextStream out(&file);
    out<<ui->textBrowser->toPlainText();
    file.close();
    // 修改窗口标题为保存文件路径
    setWindowTitle("saved:"+QFileInfo(filename).canonicalFilePath());
}
// 导入外部文件 用于文件传输
void WincomMainWindow::on_actionReadFile_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("读取文件"), 0, tr("保存为 (*.txt *.log)"));  //添加更多的文件类型
    QFile file(filename);
    //如果取消打开则退出函数
    if(file.fileName().isEmpty()){
        return;
    }
    //如果打开失败则提示并退出函数
    if(!file.open(QFile::ReadOnly | QIODevice::Text)){
        QMessageBox::warning(this, tr("打开失败"),
                                    tr("抱歉! 文件打开失败, 这可能是由于没有足够的权限造成的."),
                                    QMessageBox::Ok);
        return;
    }
    //文件大小超过限制
    if(file.size() > MAX_FILE_SIZE){
        QMessageBox::critical(this, tr("载入失败"), tr("文件大小为 %1 字节, 超过限制大小 10000 字节").arg(file.size()), QMessageBox::Ok);
        return;
    }else if(file.size() > TIP_FILE_SIZE){//文件太大时提示是否继续打开
        if(QMessageBox::question(this, tr("提示消息"), tr("您要打开的文件过大, 这将消耗更多一些的时间,要继续打开么?"),QMessageBox::Yes | QMessageBox::No) == QMessageBox::No){
            return;
        }
    }
    // 读取数据并加入到发送数据编辑框
    QByteArray line;
    while(!file.atEnd()){
        line = file.readLine();
        ui->sendMsgLineEdit->setText(ui->sendMsgLineEdit->text() + tr(line));
    }

    file.close();
    ui->statusBar->showMessage(tr("已经成功读取文件数据"));

}
// 写入文件菜单
void WincomMainWindow::on_actionWriteFile_triggered()
{
    if(ui->actionWriteFile->isChecked()){
        QString filename = QFileDialog::getSaveFileName(this, tr("写入文件"), 0, tr("保存为 (*.*)"));  //添加更多的文件类型
        if(filename.isEmpty()){
            ui->actionWriteFile->setChecked(false);
        }else{
            write2fileName = filename;
            //ui->textBrowser->setEnabled(false);
            ui->actionWriteFile->setToolTip(tr("停止写入到文件"));
        }
    }else{
        write2fileName.clear();
        //ui->textBrowser->setEnabled(true);
        ui->actionWriteFile->setToolTip(tr("将读取数据写入到文件"));
    }
}
// 计数器清零
void WincomMainWindow::on_actionClearCount_triggered()
{
    if(ui->readLcdNumber->value() == 0){// 计数器为零
        QMessageBox::information(this,tr("提示信息"),tr("计数已经清零！"),QMessageBox::Ok);
    }else{
        ui->readLcdNumber->display(0);
        ui->statusBar->showMessage(tr("计数器已经清零！"));
    }
}
// 清除串口中的I/O数控
void WincomMainWindow::on_actionClearCom_triggered()
{
    myCom->flush();
}
// 退出程序
void WincomMainWindow::on_actionExit_triggered()
{
   this->close();
}
// 单击连续发送checkBox
void WincomMainWindow::on_continueSendCheckBox_clicked()
{
    if(ui->continueSendCheckBox->isChecked()){
        ui->intervalSpinBox->setEnabled(true); // 使能“间隔时间” 可以设置间隔时间
        // 检查是否有数据，如果有则启动定时器
        ui->statusBar->showMessage(tr("启用连续发送"));
    }else{
        ui->intervalSpinBox->setEnabled(false);
        // 若定时器已经启动则关闭它
        ui->statusBar->showMessage(tr("停止连续发送"));
    }
}
//调整连续发送时间间隔
void WincomMainWindow::on_intervalSpinBox_valueChanged(int)
{
    obotimerdly = ui->intervalSpinBox->value();
}

void WincomMainWindow::on_sendHexCheckBox_clicked()
{

}

