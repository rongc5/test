#include "mainwindow.h"
#include "ipctrl.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    IPCtrl *pIPLineEdit = new IPCtrl(&w);
    pIPLineEdit->setGeometry(50, 50, 120, 26);

    w.show();

    return a.exec();
}
