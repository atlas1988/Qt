#include "wincommainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WincomMainWindow w;
    w.show();

    return a.exec();
}
