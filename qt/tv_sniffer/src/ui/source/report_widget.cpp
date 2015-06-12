
#include <QWidget>
#include <QStandardItemModel>
#include <iostream>
#include <QDateTime>
#include <QVector>
#include <QString>
#include <QPen>
#include <QBrush>
#include "data_struct.h"
#include "protocol_statistic_model.h"
#include "ip_frenquency_model.h"
#include "ip_statistics_model.h"
#include "pie_chart_widget.h"
#include "qcustomplot.h"
#include "dns_info_model.h"

#include "report_widget.h"


CReportWidget::CReportWidget ()
{
    m_ui_report.setupUi(this);
    m_protocol_statistics_model = new CProtocolStatisticModel (this);
    m_filtered_protocol_statistics_model = new CProtocolStatisticModel (this);
    m_ip_frenq_model = new CIpFrenqModel (this);
    m_filtered_ip_frenq_model = new CIpFrenqModel (this);
    m_ip_statistic_model = new CIpStatisticsModel (this);
    m_filtered_ip_statistic_model = new CIpStatisticsModel (this);
    m_dns_info_model = new CDnsInfoModel (this);
    m_filtered_dns_info_model = new CDnsInfoModel (this);
    m_ui_report.tv_protocol_statistics->setModel (m_protocol_statistics_model);
    m_ui_report.tv_filtered_protocol_stat->setModel (m_filtered_protocol_statistics_model);
    m_ui_report.tv_host_packets->setModel (m_ip_statistic_model);
    m_ui_report.tv_filtered_host_packets->setModel (m_filtered_ip_statistic_model);
    m_ui_report.tv_ip_frenquency->setModel (m_ip_frenq_model);
    m_ui_report.tv_filtered_ip_fren->setModel (m_filtered_ip_frenq_model);
    m_ui_report.tv_dns_info->setModel(m_dns_info_model);
    m_ui_report.tv_filtered_dns_info->setModel(m_filtered_dns_info_model);

	m_pie_char_wlg = new CPieChartWidget (this);
	m_filtered_pie_char_wlg = new CPieChartWidget (this);
	m_pie_char_popup_wlg =  new CPieChartWidget();
	m_filtered_pie_char_popup_wlg = new CPieChartWidget();
	m_ui_report.frame_flow_pie->layout()->addWidget(m_pie_char_wlg);

	m_ui_report.frame_filtered_flow_pie->layout()->addWidget(m_filtered_pie_char_wlg);
	m_custom_plot = new QCustomPlot (this);
	m_custom_plot->setEnabled(false);
	m_filtered_custom_plot = new QCustomPlot (this);
	m_custom_plot_popup = new QCustomPlot();
        m_filtered_custom_plot_popup = new QCustomPlot();
	m_ui_report.frame_frenquency->layout()->addWidget(m_custom_plot);
	m_ui_report.frame_filtered_frenquency->layout()->addWidget(m_filtered_custom_plot);

    clear ();
    m_ui_report.label_test_time->clear();
    m_ui_report.label_snifferid->clear();
    m_ui_report.label_userid->clear();
    m_ui_report.label_mac->clear ();
    m_ui_report.label_pf->clear ();
    m_ui_report.label_report_id->clear();
    m_ui_report.label_tv_ip->clear ();
    m_ui_report.label_pf->hide ();
    m_ui_report.label_title_ip->hide();
    m_ui_report.label_title_mac->hide ();
    m_ui_report.label_title_pf->hide ();
    m_ui_report.label_mac->hide ();
    m_ui_report.label_tv_ip->hide ();
    m_ui_report.label_pf->hide ();
    m_ui_report.tv_spaceser->changeSize (0, 0);
    m_ui_report.label_title_tv->hide ();
    m_ui_report.label_tv_right->hide ();


}
void CReportWidget::clear ()
{

    m_protocol_statistics_model->removeRows(0, m_protocol_statistics_model->rowCount());
    m_filtered_protocol_statistics_model->removeRows(0, m_filtered_protocol_statistics_model->rowCount());
    m_ip_frenq_model->removeRows(0, m_ip_frenq_model->rowCount());
    m_filtered_ip_frenq_model->removeRows(0, m_filtered_ip_frenq_model->rowCount());

    m_ip_statistic_model->removeRows(0, m_ip_statistic_model->rowCount());
    m_filtered_ip_statistic_model->removeRows(0, m_filtered_ip_statistic_model->rowCount());
    m_dns_info_model->removeRows(0, m_dns_info_model->rowCount());
    m_filtered_dns_info_model->removeRows(0, m_filtered_dns_info_model->rowCount());

}
void CReportWidget::set_ip_statistics (const std::list <IpStatisticsRecord> & ip_statistics)
{
    std::list <IpStatisticsRecord>::const_iterator cit
        = ip_statistics.begin();
    for ( ; cit != ip_statistics.end (); ++cit)
    {
        QList<QStandardItem *>  items;
        items << new QStandardItem (cit->ip.c_str ());
        QStandardItem * tmp_item = new QStandardItem ();
        tmp_item->setData(QVariant (QString(cit->packets.c_str()).toInt()), Qt::EditRole);
        items << tmp_item;
        tmp_item = new QStandardItem ();
        tmp_item->setData(QVariant (QString(cit->bytes.c_str()).toInt()), Qt::EditRole);
        items << tmp_item;
        m_ip_statistic_model->appendRow(items);
    }
}
void CReportWidget::set_protocol_statistics (const std::list <ProtocolStatistics> & protocol_statistics) 
{
    std::list <ProtocolStatistics>::const_iterator cit = protocol_statistics.begin();
    for ( ; cit != protocol_statistics.end (); ++cit)
    {
       
        QList<QStandardItem *>  items;
        items << new QStandardItem (cit->protocol.c_str ());
        QStandardItem * tmp_item = new QStandardItem ();
        tmp_item->setData(QVariant (cit->count), Qt::EditRole);
        items << tmp_item;
        m_protocol_statistics_model->appendRow(items);
    }
}
void CReportWidget::set_frenquency (const std::list <HostFrenquency> & host_frenquency)
{
    m_ip_frenq_model->clear();
    std::cout << "CReportWidget::set_frenquency\n";
    if (!host_frenquency.empty())
    {
        std::cout << "set_frenquency size = " << host_frenquency.begin()->count.size() << "\n";
    }
    
    std::list <HostFrenquency>::const_iterator cit = host_frenquency.begin();
    for ( ; cit != host_frenquency.end (); ++cit)
    {
        if (cit == host_frenquency.begin())
        {
            QStringList head_list;
            head_list << "ip";
            int count = 0;
            
            std::list <int>::const_iterator cit_int = cit->count.begin();
            for ( ; cit_int != cit->count.end (); ++cit_int )
            {
                QString head;
                head.append(QString::fromLocal8Bit("开始"));
                head.append(QString::number((++count) * 5));
                head.append(QString::fromLocal8Bit("分钟次数"));
                head_list << head;
            }
           
            m_ip_frenq_model->setHorizontalHeaderLabels (head_list);
        }
        QList<QStandardItem *>  items;
        items << new QStandardItem (cit->host.c_str ());
         std::list <int>::const_iterator cit_int = cit->count.begin();
         for ( ; cit_int != cit->count.end (); ++cit_int )
         {
             QStandardItem * tmp_item = new QStandardItem ();
             tmp_item->setData(QVariant (*cit_int), Qt::EditRole);
             items << tmp_item;
         }
        m_ip_frenq_model->appendRow(items);
    }
}
void CReportWidget::set_flow_pie_data (const std::list <FlowPieData> & flow_pie_data)
{
    std::list <PieElement> list_pie_element;
    std::list <FlowPieData>::const_iterator cit = 
        flow_pie_data.begin();
    for ( ; cit != flow_pie_data.end (); ++cit)
    {
        PieElement element;
        element.name = cit->ip;
        element.data = cit->data;
        list_pie_element.push_back(element);
    }
    m_pie_char_wlg->set_pie_elements(list_pie_element);
m_pie_char_popup_wlg->set_pie_elements(list_pie_element);
}
void CReportWidget::set_filtered_ip_statistics (const std::list <IpStatisticsRecord> & ip_statistics)
{
    std::list <IpStatisticsRecord>::const_iterator cit
        = ip_statistics.begin();
    for ( ; cit != ip_statistics.end (); ++cit)
    {
        QList<QStandardItem *>  items;
        items << new QStandardItem (cit->ip.c_str ());
        QStandardItem * tmp_item = new QStandardItem ();
        tmp_item->setData(QVariant (QString(cit->packets.c_str()).toInt()), Qt::EditRole);
        items << tmp_item;
        tmp_item = new QStandardItem ();
        tmp_item->setData(QVariant (QString(cit->bytes.c_str()).toInt()), Qt::EditRole);
        items << tmp_item;
        m_filtered_ip_statistic_model->appendRow(items);
    }
}
void CReportWidget::set_filtered_protocol_statistics (const std::list <ProtocolStatistics> & protocol_statistics)
{
    std::list <ProtocolStatistics>::const_iterator cit = protocol_statistics.begin();
    for ( ; cit != protocol_statistics.end (); ++cit)
    {

        QList<QStandardItem *>  items;
        items << new QStandardItem (cit->protocol.c_str ());
        QStandardItem * tmp_item = new QStandardItem ();
        tmp_item->setData(QVariant (cit->count), Qt::EditRole);
        items << tmp_item;
        m_filtered_protocol_statistics_model->appendRow(items);
    }
}
void CReportWidget::set_filtered_frenquency (const std::list <HostFrenquency> & host_frenquency)
{
    m_filtered_ip_frenq_model->clear();
    std::list <HostFrenquency>::const_iterator cit = host_frenquency.begin();
    for ( ; cit != host_frenquency.end (); ++cit)
    {
        if (cit == host_frenquency.begin())
        {
            QStringList head_list;
            head_list << "ip";
            int count = 0;
            std::list <int>::const_iterator cit_int = cit->count.begin();
            for ( ; cit_int != cit->count.end (); ++cit_int )
            {
                QString head;
                head.append(QString::fromLocal8Bit("开始"));
                head.append(QString::number((++count) * 5));
                head.append(QString::fromLocal8Bit("分钟次数"));
                head_list << head;
            }
            m_filtered_ip_frenq_model->setHorizontalHeaderLabels (head_list);
        }
        QList<QStandardItem *>  items;
        items << new QStandardItem (cit->host.c_str ());
        std::list <int>::const_iterator cit_int = cit->count.begin();
        for ( ; cit_int != cit->count.end (); ++cit_int )
        {
            QStandardItem * tmp_item = new QStandardItem ();
            tmp_item->setData(QVariant (*cit_int), Qt::EditRole);
            items << tmp_item;
        }
        m_filtered_ip_frenq_model->appendRow(items);
    }
}
void CReportWidget::set_filtered_flow_pie_data (const std::list <FlowPieData> & flow_pie_data)
{
    std::list <PieElement> list_pie_element;
    std::list <FlowPieData>::const_iterator cit = 
        flow_pie_data.begin();
    for ( ; cit != flow_pie_data.end (); ++cit)
    {
        PieElement element;
        element.name = cit->ip;
        element.data = cit->data;
        list_pie_element.push_back(element);
    }
    m_filtered_pie_char_wlg->set_pie_elements(list_pie_element);
	m_filtered_pie_char_popup_wlg->set_pie_elements(list_pie_element);
}
void CReportWidget::set_basic_info (const BasicInfo & basic_info)
{
    m_ui_report.label_test_time->clear();
    m_ui_report.label_snifferid->clear();
    m_ui_report.label_userid->clear();
    m_ui_report.label_mac->clear ();
    m_ui_report.label_pf->clear ();
    m_ui_report.label_report_id->clear();
    m_ui_report.label_tv_ip->clear ();
    m_ui_report.label_test_time->setText(basic_info.time.c_str());
    m_ui_report.label_snifferid->setText(basic_info.sniffer_id.c_str());
    m_ui_report.label_userid->setText(basic_info.user_id.c_str());
    m_ui_report.label_mac->setText(basic_info.tv_Mac.c_str());
    m_ui_report.label_pf->setText (basic_info.pf.c_str ());
    m_ui_report.label_report_id->setText (basic_info.report_id.c_str ());
    std::cout << "CReportWidget::set_basic_info basic_info.tv_ip = " << basic_info.tv_ip.c_str () << "\n";
    m_ui_report.label_tv_ip->setText (basic_info.tv_ip.c_str ());
}
void CReportWidget::set_wave_statistics (const std::list <WaveData> & wave_statistics, const QDateTime & start, const QDateTime & end)
{	
    std::list <WaveData>::const_iterator wave_cit
        = wave_statistics.begin();
        
  m_custom_plot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
  m_custom_plot->clearGraphs();
  
  int MaxY = 0;
  uint time_start = start.toTime_t(), time_end = end.toTime_t();
  int length, index;


  for (int i = 1, index = 0;  wave_cit != wave_statistics.end () && (length = wave_cit->count.size()); ++wave_cit, ++i, index = 0)
  {
        if (i >= 6)
  		break;
    m_custom_plot->addGraph();
    //QPen pen;
    //pen.setColor(QColor(0, 0, 255, 200));
    m_custom_plot->graph()->setLineStyle(QCPGraph::lsLine);
    //m_custom_plot->graph()->setPen(pen);
    //m_custom_plot->graph()->setBrush(QBrush(QColor(255/10.0*i,160,50,150)));
    m_custom_plot->graph()->setPen(QPen(QColor(255/10.0*i,160,50,150)));
    m_custom_plot->graph()->setName(wave_cit->host.c_str());
    
    //printf("host.c_str():%s\n", wave_cit->host.c_str());

    QVector <double> x, y;

    x.clear();
    y.clear();

    std::list <int>::const_iterator cit = wave_cit->count.begin();
    index = (time_end - time_start)/(length);

    for (int j = 1 ; cit != wave_cit->count.end (); ++cit, ++j)
    {
         x.push_back(time_start+ index*j);

         y.push_back(*cit);

         if (*cit > MaxY)
          MaxY = *cit;
         //printf("MaxY:%d, %d\n", index, *cit);
    }

    m_custom_plot->graph()->setData(x, y);
  } 

  m_custom_plot->xAxis->setLabel("时间");
  m_custom_plot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  m_custom_plot->xAxis->setDateTimeFormat("hh:mm:ss\nyyyy/MM/dd");
  //m_custom_plot->xAxis->setDateTimeFormat("MM-dd hh:mm:ss");
  m_custom_plot->yAxis->setLabel("包的次数");
  
  //m_custom_plot->xAxis->setAutoTicks(false); 
  //m_custom_plot->xAxis->setAutoTicks(false);

  m_custom_plot->xAxis->setTickStep(1);
  m_custom_plot->yAxis->setTickStep(1);
  
  //m_custom_plot->xAxis->setSubTickCount(0);
  //m_custom_plot->yAxis->setSubTickCount(0);

  // make top and right axes visible but without ticks and labels:

  m_custom_plot->xAxis2->setTicks(false);
  m_custom_plot->yAxis2->setTicks(false);

  m_custom_plot->xAxis2->setTickLabels(false);
  m_custom_plot->yAxis2->setTickLabels(false);

  m_custom_plot->xAxis->setRange(time_start, time_end);
  m_custom_plot->yAxis->setRange(0, MaxY);
  
  m_custom_plot->xAxis2->setSubTickCount(0);
  m_custom_plot->yAxis2->setSubTickCount(0);

  m_custom_plot->legend->setVisible(true);
  m_custom_plot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

  m_custom_plot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

  m_custom_plot->repaint ();
  m_custom_plot->replot();

}

void CReportWidget::set_filtered_wave_statistics (const std::list <WaveData> & wave_statistics, const QDateTime & start, const QDateTime & end)
{	  
  std::list <WaveData>::const_iterator wave_cit
      = wave_statistics.begin();

m_filtered_custom_plot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
m_filtered_custom_plot->clearGraphs();

int MaxY = 0;
uint time_start = start.toTime_t(), time_end = end.toTime_t();
int length, index;

for (int i = 1, index = 0; wave_cit != wave_statistics.end () && (length = wave_cit->count.size()); ++wave_cit, ++i, index = 0)
{
      if (i >= 6)
              break;
  m_filtered_custom_plot->addGraph();
  m_filtered_custom_plot->graph()->setLineStyle(QCPGraph::lsLine);
  m_filtered_custom_plot->graph()->setPen(QPen(QColor(255/10.0*i,160,50,150)));
  m_filtered_custom_plot->graph()->setName(wave_cit->host.c_str());

  //printf("host.c_str():%s\n", wave_cit->host.c_str());

  QVector <double> x, y;

  x.clear();
  y.clear();

  std::list <int>::const_iterator cit = wave_cit->count.begin();

  index = (time_end - time_start)/(length);

  for (int j = 1; cit != wave_cit->count.end (); ++cit, ++j)
  {
       x.push_back(time_start+ index*j);

       y.push_back(*cit);

       if (*cit > MaxY)
        MaxY = *cit;
       //printf("MaxY:%d, %d\n", index, *cit);
  }

  m_filtered_custom_plot->graph()->setData(x, y);
}

m_filtered_custom_plot->xAxis->setLabel("时间");
m_filtered_custom_plot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
m_filtered_custom_plot->xAxis->setDateTimeFormat("hh:mm:ss\nyyyy/MM/dd");
//m_custom_plot->xAxis->setDateTimeFormat("MM-dd hh:mm:ss");
m_filtered_custom_plot->yAxis->setLabel("包的次数");

//m_custom_plot->xAxis->setAutoTicks(false);
//m_custom_plot->xAxis->setAutoTicks(false);

m_filtered_custom_plot->xAxis->setTickStep(1);
m_filtered_custom_plot->yAxis->setTickStep(1);

//m_custom_plot->xAxis->setSubTickCount(0);
//m_custom_plot->yAxis->setSubTickCount(0);

// make top and right axes visible but without ticks and labels:

m_filtered_custom_plot->xAxis2->setTicks(false);
m_filtered_custom_plot->yAxis2->setTicks(false);

m_filtered_custom_plot->xAxis2->setTickLabels(false);
m_filtered_custom_plot->yAxis2->setTickLabels(false);

m_filtered_custom_plot->xAxis->setRange(time_start, time_end);
m_filtered_custom_plot->yAxis->setRange(0, MaxY);

m_filtered_custom_plot->xAxis2->setSubTickCount(0);
m_filtered_custom_plot->yAxis2->setSubTickCount(0);

m_filtered_custom_plot->legend->setVisible(true);
m_filtered_custom_plot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
m_filtered_custom_plot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

m_filtered_custom_plot->repaint ();
m_filtered_custom_plot->replot();

}
void CReportWidget::set_dns_statistics (const std::list <DnsInfo> & dns_statistics)
{
    std::list <DnsInfo>::const_iterator cit
        = dns_statistics.begin();
    for ( ; cit != dns_statistics.end (); ++cit)
    {
        QList<QStandardItem *>  items;
        items << new QStandardItem (cit->domain_name.c_str ());
        items << new QStandardItem (cit->ip.c_str ());
        m_dns_info_model->appendRow(items);
    }
    
}
void CReportWidget::set_filtered_dns_statistics (const std::list <DnsInfo> & dns_statistics)
{
    std::list <DnsInfo>::const_iterator cit
        = dns_statistics.begin();
    for ( ; cit != dns_statistics.end (); ++cit)
    {
        QList<QStandardItem *>  items;
        items << new QStandardItem (cit->domain_name.c_str ());
        items << new QStandardItem (cit->ip.c_str ());
        m_filtered_dns_info_model->appendRow(items);
    }
}
void CReportWidget::set_report_statistics (const ReportStatInfo & report_stat_info)
{
    clear();
    set_ip_statistics (report_stat_info.ip_statistics);
    set_filtered_ip_statistics(report_stat_info.filtered_ip_statistics);

    set_protocol_statistics(report_stat_info.protocol_statistics);
    set_filtered_protocol_statistics (report_stat_info.filtered_protocol_statistics);

    set_frenquency(report_stat_info.host_frenquency);
    set_filtered_frenquency (report_stat_info.filtered_host_frenquency);

    set_flow_pie_data(report_stat_info.flow_pie_data);
    set_filtered_flow_pie_data (report_stat_info.filtered_flow_pie_data);

    set_wave_statistics (report_stat_info.wave_statistics, report_stat_info.start, report_stat_info.end);
    set_filtered_wave_statistics (report_stat_info.filtered_wave_statistics, report_stat_info.start, report_stat_info.end);

    set_dns_statistics (report_stat_info.dns_statistics);
    set_filtered_dns_statistics (report_stat_info.filtered_dns_statistics);
}
Ui_report *CReportWidget::get_ui_report()
{
        return &m_ui_report;
}
void CReportWidget::grab_pip_and_wave()
{
	m_pie_char_popup_wlg->show();
	QPixmap piePixmap(get_ui_report()->frame_flow_pie->width(), get_ui_report()->frame_flow_pie->height());
	get_ui_report()->frame_flow_pie->render(&piePixmap);
	piePixmap.save(QString("/tmp/pie.jpg"), "JPG");
	m_pie_char_popup_wlg->hide();


	m_filtered_pie_char_popup_wlg->show();
	QPixmap filteredPiePixmap(get_ui_report()->frame_filtered_flow_pie->width(), get_ui_report()->frame_filtered_flow_pie->height());
	get_ui_report()->frame_filtered_flow_pie->render(&filteredPiePixmap);
	filteredPiePixmap.save(QString("/tmp/filtered_pie.jpg"), "JPG");
	m_filtered_pie_char_popup_wlg->hide();

	m_custom_plot_popup->show();
	QPixmap wavePixmap(get_ui_report()->frame_frenquency->width(), get_ui_report()->frame_frenquency->height());
	get_ui_report()->frame_frenquency->render(&wavePixmap);
	wavePixmap.save(QString("/tmp/wave.jpg"), "JPG");
	m_custom_plot_popup->hide();


	m_filtered_custom_plot_popup->show();
	QPixmap filteredWavePixmap(get_ui_report()->frame_filtered_frenquency->width(), get_ui_report()->frame_filtered_frenquency->height());
	get_ui_report()->frame_filtered_frenquency->render(&filteredWavePixmap);
	filteredWavePixmap.save(QString("/tmp/filtered_wave.jpg"), "JPG");
	m_filtered_custom_plot_popup->hide();

}
