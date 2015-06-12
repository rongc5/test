#include <QTableView>
#include <QTimerEvent>
#include "packet_list_view.h"

CPacketListView::CPacketListView (QWidget *parent) 
    : QTableView (parent), m_scroll_bottom (false)
{
    m_timer_id = startTimer(1000 * 1);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
}
CPacketListView::~CPacketListView ()
{
    if (m_timer_id != 0)
    {
        killTimer(m_timer_id);
    }
}
void CPacketListView::timerEvent(QTimerEvent * event)
{
    
    if (event->timerId () == m_timer_id && m_scroll_bottom)
    {
        scrollToBottom();
        resizeColumnToContents(1);
    }
    else
    {
        QTableView::timerEvent (event);
    }
}
void CPacketListView::stop_scroll ()
{
    m_scroll_bottom = false;
}
void CPacketListView::start_scroll ()
{
    m_scroll_bottom = true;
}