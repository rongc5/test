
#include <iostream>
#include "iview.h"
#include "view.h"
#include "ictrl.h"
#include "main_window.h"
#include "data_struct.h"
#include "packet_list_model.h"
#include "histogram_widget.h"
#include "packet_list_view.h"
#include "report_widget.h"

IView * get_view ()
{
    return new CView ();
}

CView::CView () : 
m_ctrl (NULL),
    m_main_win (NULL),
    m_packet_list_model (NULL),
    m_histogram_widget (NULL),
    m_packet_list_view (NULL),
    m_report_widget (NULL)
{
   
}

void CView::init (ICtrl * ctrl)
{
    m_ctrl = ctrl;
}
void CView::set_widget (QWidget * widget)
{
    m_main_win = widget;
    CMainWin * win_main = dynamic_cast <CMainWin*>(m_main_win);
    m_packet_list_model = win_main->get_packet_list_model ();
    m_histogram_widget = win_main->get_histogram_widget();
    m_packet_list_view = win_main->get_packet_list_view ();
    m_report_widget = win_main->get_report_widget();
}
void CView::clear_packet_list ()
{
    if (m_packet_list_model == NULL)
    {
        return;
    }
   m_packet_list_model->clear ();
}
void CView::add_packet (const Packet_Record & packet_record)
{
    if (m_packet_list_model == NULL)
    {
        return;
    }
    m_packet_list_model->add_packet (packet_record);
    
}
void CView::release ()
{
    delete this;
}
void CView::start_capture ()
{
    m_ctrl->start_capture();
    m_packet_list_view->start_scroll();
    m_histogram_widget->clear();
}
void CView::stop_capture () 
{
    m_ctrl->stop_capture();
    m_packet_list_view->stop_scroll();
}
void CView::update ()
{
     CMainWin * win_main = dynamic_cast <CMainWin*>(m_main_win);
     win_main->update ();
}
void CView::add_packet_count (int packet_count)
{
    m_histogram_widget->add_packet_count(packet_count);
}

void CView::set_ip_statistics (const std::list <IpStatisticsRecord> & ip_statistics)
{
    m_report_widget->set_ip_statistics(ip_statistics);
}
void CView::set_protocol_statistics (const std::list <ProtocolStatistics> & protocol_statistics) 
{
    m_report_widget->set_protocol_statistics (protocol_statistics);
}
void CView::set_frenquency (const std::list <HostFrenquency> & host_frenquency) 
{
    m_report_widget->set_frenquency (host_frenquency);
}
void CView::set_flow_pie_data (const std::list <FlowPieData> & flow_pie_data) 
{
    m_report_widget->set_flow_pie_data (flow_pie_data);
}
void CView::set_filtered_ip_statistics (const std::list <IpStatisticsRecord> & ip_statistics)
{
    m_report_widget->set_filtered_ip_statistics (ip_statistics);
}
void CView::set_filtered_protocol_statistics (const std::list <ProtocolStatistics> & protocol_statistics) 
{
    m_report_widget->set_filtered_protocol_statistics (protocol_statistics);
}
void CView::set_filtered_frenquency (const std::list <HostFrenquency> & host_frenquency) 
{
    m_report_widget->set_filtered_frenquency (host_frenquency);
}
void CView::set_filtered_flow_pie_data (const std::list <FlowPieData> & flow_pie_data) 
{
    m_report_widget->set_filtered_flow_pie_data (flow_pie_data);
}

void CView::set_basic_info (const BasicInfo & basic_info) 
{
    m_report_widget->set_basic_info (basic_info);
}

void CView::set_filter (const std::list <IpFilterInfo> & ip_filter_info)
{
    m_ctrl->set_filter(ip_filter_info);
}
 void CView::set_wave_statistics (const std::list <WaveData> & wave_statistics)
{
    //m_report_widget->set_wave_statistics(wave_statistics);
}
 void CView::set_filtered_wave_statistics (const std::list <WaveData> & wave_statistics)
{
    //m_report_widget->set_filtered_wave_statistics(wave_statistics);
}
  void CView::set_report_statistics (const ReportStatInfo & report_stat_info)
 {
     m_report_widget->set_report_statistics(report_stat_info);
 }
  bool CView::save_filter_record_to_file(const std::vector<FilterRecord> & v_filter_record)
  {
      return m_ctrl->save_filter_record_to_file(v_filter_record);
  }
  bool CView::get_filter_record_from_file (std::vector<FilterRecord> & v_filter_record)
  {
      std::cout << "get_filter_record_from_file\n";
      return m_ctrl->get_filter_record_from_file(v_filter_record);
  }
CReportWidget *CView::get_report_widget()
{
        return m_report_widget;
}

void CView::grab_pie_wave()
{
        m_report_widget->grab_pip_and_wave();
}
