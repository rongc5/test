//#include "mainwindow.h"
#include "ipctrl.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QMainWindow mainwindow;
    mainwindow.setFixedSize(300, 200);

    ipctrl *pIPLineEdit = new ipctrl(&mainwindow);
    pIPLineEdit->setGeometry(50, 50, 120, 26);

    mainwindow.show();
    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));

    return a.exec();
}
