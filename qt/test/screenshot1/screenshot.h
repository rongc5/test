#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QPixmap>
#include <QWidget>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE


class Screenshot : public QWidget
{
    Q_OBJECT
public:

        Screenshot();
        void shootScreen();

private:
    QPixmap originalPixmap;
};


#endif
