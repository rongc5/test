#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QThread>
#include "base.h"

class MyThread : public QThread
{
    Q_OBJECT
protected:
    void run();
private:
    Basic info;
};

#endif // MYTHREAD_H
