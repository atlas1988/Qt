#include "wincommainwindow.h"
#include <QApplication> // 加入头文件 支持中文

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 使程序可以处理中文
    QTextCodec::setCodecForLocale(QTextCodec::codecForName(QTextCodec::codecForLocale()->name()));
    WincomMainWindow w;
    w.show();

    return a.exec();
}
