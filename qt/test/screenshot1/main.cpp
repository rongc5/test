#include <QApplication>

#include "screenshot.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Screenshot screenshot;
    screenshot.show();
    screenshot.shootScreen();
    return app.exec();
}
