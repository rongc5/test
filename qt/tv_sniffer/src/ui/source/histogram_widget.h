/** 
 * Copyright (C) 2007-2013 Haier
 *
 * @file             histogram_widget.h
 * @brief            直方图widget
 * @author           zhengyl
 * @version	         1.0
 * @date	         2014-1-24
 */
#ifndef _HISTOGRAM_H_
#define _HISTOGRAM_H_
#include <list>
class QWidget;
class QPaintEvent;
class QRect;

class CHistogram_Widget : public QWidget
{
public:
    CHistogram_Widget ();
    /** 增加包数数据
     * 
     * @param  packet_count
     * @return 
     * @see 
     * @note 
     */
    void add_packet_count (int packet_count);
    /** 清空
     * 
     * @return 
     * @see 
     * @note 
     */
    void clear ();
protected:
    /** 画图数据
     * 
     * @param  event
     * @return 
     * @see 
     * @note 
     */
    virtual void paintEvent (QPaintEvent * event);
private:
    /** 获取矩形区域
     * 
     * @param  rect
     * @return 
     * @see 
     * @note 
     */
    void get_rect (QRect & rect);
private:

static const int EDGE;
static const int WIDTH_PER_COLUMN;
static const int MAX_PACKET_COUNT;
private:
    std::list<int>  m_packet_count_list;

};

#endif