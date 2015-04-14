#include <QtGui>

#include "screenshot.h"

Screenshot::Screenshot()
{
    setWindowTitle(tr("Screenshot"));
    resize(1000, 1000);
}

void Screenshot::shootScreen()
{
    originalPixmap = QPixmap(); // clear image for low memory situations
                                // on embedded devices.
    //originalPixmap = QPixmap::grabWindow(QApplication::desktop()->winId());
    //originalPixmap = QPixmap::grabWindow(QWidget::winId());
    originalPixmap = QPixmap::grabWindow(this->winId());

    originalPixmap.save(QString("/mnt/hgfs/D/code/linux-virtual/myproj/qt/test/screenshot1/hello.jpg"), "JPG");
}

