
#include <QWidget>
#include "pie_chart_widget.h"
#include "nightcharts.h"
#include "data_struct.h"


CPieChartWidget::CPieChartWidget (QObject* parent)
{

}
void CPieChartWidget::set_pie_elements (const std::list <PieElement> & list_pie_element)
{
    m_list_pie_element = list_pie_element;
    update(this->geometry());
}
void CPieChartWidget::paintEvent(QPaintEvent * e)
{
    QWidget::paintEvent(e);
    QPainter painter(this);
    QFont font;
    Nightcharts PieChart;
    PieChart.setType(Nightcharts::Dpie);//{Histogramm,Pie,DPie};
    PieChart.setLegendType(Nightcharts::Vertical);//{Round,Vertical}
    PieChart.setCords(10,10,this->width()/1.7,this->height()/1.5);

    std::list <PieElement>::const_iterator cit = m_list_pie_element.begin();
    int count = 0;
    if (m_list_pie_element.empty())
    {
        return;
    }
     int index = 0;
    for ( ; cit != m_list_pie_element.end (); ++cit)
    {
        QColor color = Qt::green;
        ++index;
       
        if (index == 1)
        {
            color = QColor(200,10,50);
        }
        if (index == 2)
        {
            color = Qt::green;
        }
        if (index == 3)
        {
            color = Qt::cyan;
        }
        if (index == 4)
        {
            color = Qt::yellow;
        }
        if (index == 5)
        {
            color = Qt::blue;
        }
        if (index == 6)
        {
            color = Qt::darkCyan;
        }
        if (index == 7)
        {
            color = Qt::color1;
        }
        if (index == 8)
        {
            color = Qt::lightGray;
        }
        if (index == 9)
        {
            color = Qt::darkMagenta;
        }

        if (index == 10)
        {
            color = Qt::darkYellow;
        }



        PieChart.addPiece(cit->name.c_str(), color, cit->data);
    }

  /*PieChart.addPiece("Item1",QColor(200,10,50),34);
    PieChart.addPiece("Item2",Qt::green,27);
    PieChart.addPiece("Item3",Qt::cyan,14);
    PieChart.addPiece("Item4",Qt::yellow,7);
    PieChart.addPiece("Item5",Qt::blue,4);*/
    PieChart.draw(&painter);
    PieChart.drawLegend(&painter);
}