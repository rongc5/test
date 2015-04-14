/** 
 * Copyright (C) 2007-2013 Haier
 *
 * @file             pie_chart_widget.h
 * @brief            ±ýÍ¼wlg
 * @author           zhengyl
 * @version	         1.0
 * @date	         2014-1-24
 */
#ifndef _PIE_CHART_WIDGET_H_
#define _PIE_CHART_WIDGET_H_

class QWidget;
class QPaintEvent;
class QObject;
struct PieElement;

class CPieChartWidget : public QWidget
{
public:
    CPieChartWidget (QObject* parent = 0);
    void set_pie_elements (const std::list <PieElement> & list_pie_element);
protected:
    virtual void paintEvent(QPaintEvent *);
private:
    std::list <PieElement> m_list_pie_element;
};









#endif