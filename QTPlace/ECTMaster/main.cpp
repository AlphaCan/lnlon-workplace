#include "ethercatmaster.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EthercatMaster w;
    w.show();

    return a.exec();
}
