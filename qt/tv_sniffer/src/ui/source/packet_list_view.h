/** 
 * Copyright (C) 2007-2013 Haier
 *
 * @file             packet_list_view.h
 * @brief            packetlist view
 * @author           zhengyl
 * @version	         1.0
 * @date	         2014-1-24
 */
#ifndef _PACKET_LIST_VIEW_H_
#define _PACKET_LIST_VIEW_H_
class QTableView;
class QTimerEvent;
class QWidget;

class CPacketListView : public QTableView
{
public:
    CPacketListView (QWidget *parent = 0);
    ~CPacketListView ();
    /** 停止滚动
     * 
     * @return void
     * @see 
     * @note 
     */
    void stop_scroll ();
    /** 开始滚动
     * 
     * @return void
     * @see 
     * @note 
     */
    void start_scroll ();
protected:
    /** 时间事件
     * 
     * @param  event 时间事件
     * @return void
     * @see 
     * @note 
     */
    void timerEvent(QTimerEvent * event);
private:
    bool m_scroll_bottom;
    int m_timer_id;
};


#endif