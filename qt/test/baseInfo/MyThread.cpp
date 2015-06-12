#include "MyThread.h"
#include <QtDebug>

    //QString haierIp;
    //QString haierMac;
    //QString snifferIp;
    //QString snifferNetface;

void MyThread::run()
{

    Binfo base;
    while (1){
    //printf("hello world\n");
    info.setBasic();
    info.getBasic(&base);
    qDebug() << base.haierIp << base.haierMac << base.snifferIp << base.snifferNetface;
    //msleep(1000);
    }
}
