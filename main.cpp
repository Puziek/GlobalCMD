#include "globalcmd.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GlobalCMD w;
    w.show();

    return a.exec();
}
