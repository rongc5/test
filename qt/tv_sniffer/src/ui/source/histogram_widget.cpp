
#include <QWidget>
#include <QPainter>
#include <QList>
#include <QPen>
#include "histogram_widget.h"


const int CHistogram_Widget::EDGE = 10;
const int CHistogram_Widget::WIDTH_PER_COLUMN = 15;
const int CHistogram_Widget::MAX_PACKET_COUNT = 1000;

CHistogram_Widget::CHistogram_Widget ()
{


}
void CHistogram_Widget::paintEvent(QPaintEvent * event)
{
    QPainter painter (this);
    QRect rect;
    get_rect (rect);
    
    for (int i = 0; i < 4; ++i)
    {
        QString text(QString::number (MAX_PACKET_COUNT / 4 * (i + 1)));
        QFontMetrics fm (painter.font ());
        QRect fr = fm.boundingRect(text);
        painter.drawText(rect.left() - fr.width(),
            fm.height() + (rect.height() - fm.height()) / 4 * (4 - i - 1) + fm.height(),
            text);
    }
    QString title(QString::fromLocal8Bit("每秒钟包数"));
    QFontMetrics fm (painter.font ());
    QRect fr = fm.boundingRect(title);
    painter.drawText(rect.center().x () - fr.width() / 2, rect.top(), title);

    painter.drawLine(rect.bottomLeft(), rect.bottomRight());
    painter.drawLine(rect.bottomLeft(), rect.topLeft());
    std::list<int>::reverse_iterator rit = m_packet_count_list.rbegin();
    int count = 0;
    for ( ; rit != m_packet_count_list.rend(); ++rit)
    {
        if (rect.left() + count * WIDTH_PER_COLUMN + WIDTH_PER_COLUMN > rect.right())
        {
            break;
        }
        int column_height = (double) (*rit) / MAX_PACKET_COUNT * rect.height();
        if (column_height > rect.height())
        {
            column_height = rect.height();
        }
        painter.fillRect(QRect (rect.left() + count * WIDTH_PER_COLUMN,
            rect.bottom() - column_height,
            WIDTH_PER_COLUMN, 
            column_height), QColor (0, 128, 0));

        QPen old_pen =  painter.pen ();
        QPen new_pen (old_pen);
        new_pen.setColor(Qt::black);
        painter.setPen(new_pen);
        painter.drawRect(QRect (rect.left() + count * WIDTH_PER_COLUMN,
            rect.bottom() - column_height,
            WIDTH_PER_COLUMN, 
            column_height ));
        painter.setPen(old_pen);
        ++count;
    }
}
void CHistogram_Widget::get_rect (QRect & rect)
{
    if (width () < EDGE * 2 ||height() < EDGE * 2)
    {
        return;
    }
    QFontMetrics fm (font ());
    QRect fr = fm.boundingRect(QString::number(MAX_PACKET_COUNT));

    rect = QRect (fr.width() + 2 , fm.height() + 4, 
        width() - EDGE * 2 - fr.width() - 2, height() - EDGE * 2 - fm.height() - 4);
}
void CHistogram_Widget::add_packet_count (int packet_count)
{
   
    m_packet_count_list.push_back(packet_count);
    if (m_packet_count_list.size () > 1000)
    {
        m_packet_count_list.pop_front();
    }
    update(geometry ());
}
void CHistogram_Widget::clear ()
{
    m_packet_count_list.clear();
    update(geometry ());
}