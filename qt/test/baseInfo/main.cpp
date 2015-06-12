#include <QCoreApplication>
#include <QThread>
#include "MyThread.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MyThread *p = new MyThread();
    p->start();

    return a.exec();
}
