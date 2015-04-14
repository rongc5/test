
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <iostream>

#include "start_button.h"
#include "start_page.h"

CStartPage::CStartPage (QWidget * parent)
    : QWidget (parent)
{
    std::cout << "m_ui.setupUi\n";
    m_ui.setupUi(this);
    std::cout << "m_ui.setupUi over\n";

    QPixmap pixmap(QString::fromUtf8(":/images/start_big.png"));
    m_start_button = new CStartButton (pixmap);

    m_gw = new QGraphicsView (this);
    std::cout << "new QGraphicsView \n";
    {
        QPalette palette_centre = m_gw->palette();
        m_gw->setAutoFillBackground(true);
        palette_centre.setBrush(m_gw->backgroundRole(), QBrush(QColor(0, 0, 0, 0)));
       m_gw->setPalette(palette_centre);
    }
   std::cout << "new QGraphicsView  over\n";
    m_scene = new QGraphicsScene;
    m_gw->setScene(m_scene);
    std::cout << "setScene  over\n";
    m_scene->addItem(m_start_button);
     std::cout << "addItem  over\n";
    m_ui.frame_btn->layout()->addWidget(m_gw);
    std::cout << "addWidget  over\n";
    QPalette palette_centre = m_ui.frame_btn->palette();
    m_ui.frame_btn->setAutoFillBackground(true);
    palette_centre.setBrush(backgroundRole(), QBrush(QColor(0, 0, 0, 0)));
    //palette_centre.setBrush(foregroundRole(), QBrush(QColor(0, 0, 0, 0)));
    m_ui.frame_btn->setPalette(palette_centre);
    connect(m_start_button, SIGNAL(released()), this, SIGNAL(start()));
    std::cout << "CStartPage over";
   // m_ui.frame_btn->setBackgroundRole()
}
 void CStartPage::paintEvent(QPaintEvent * event)
{
    QPainter painter (this);
    QPixmap pixmap(QString::fromUtf8(":/images/background.png"));
    painter.drawPixmap(0, 0, width (),height(), pixmap);
}