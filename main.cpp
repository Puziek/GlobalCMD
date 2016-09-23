#include "globalcmd.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("PuzotronSoft");
    QCoreApplication::setApplicationName("GlobalCMD");

    GlobalCMD w;
    w.show();

    return a.exec();
}
