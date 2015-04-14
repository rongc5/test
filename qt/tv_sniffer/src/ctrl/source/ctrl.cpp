
#include <iostream>

#include "ictrl.h"
#include "ctrl.h"
#include "imodel.h"
#include "data_struct.h"
#include "iview.h"

ICtrl * get_ctrl_ptr ()
{
    return new CCtrl ();
}
CCtrl::CCtrl () : m_model (0), m_view (0)
{

}
void CCtrl::init (IModel * model, IView * view)
{
    m_model = model;
    m_view = view;
}
void CCtrl::start_capture ()
{
    m_model->start_capture();
}
void CCtrl::stop_capture ()
{
    m_model->stop_capture();
}
 void CCtrl::clear_packet_list ()
 {
     m_view->clear_packet_list();
 }
 void CCtrl::add_packet (const Packet_Record & packet_record)
 {
     m_view->add_packet(packet_record);
 }
  void CCtrl::update_view ()
  {
      m_view->update();
  }
   void CCtrl::add_packet_count (int packet_count)
  {
      m_view->add_packet_count(packet_count);
  }
 void CCtrl::set_ip_statistics (const std::list <IpStatisticsRecord> & ip_statistics)
{
    m_view->set_ip_statistics (ip_statistics);
}
void CCtrl::set_protocol_statistics (const std::list <ProtocolStatistics> & protocol_statistics)
{
    m_view->set_protocol_statistics (protocol_statistics);
}
void CCtrl::set_frenquency (const std::list <HostFrenquency> & host_frenquency)
{
    m_view->set_frenquency (host_frenquency);
}
void CCtrl::set_flow_pie_data (const std::list <FlowPieData> & host_frenquency)
{
    m_view->set_flow_pie_data (host_frenquency);
}
void CCtrl::set_filtered_ip_statistics (const std::list <IpStatisticsRecord> & ip_statistics)
{
    m_view->set_filtered_ip_statistics (ip_statistics);
}
void CCtrl::set_filtered_protocol_statistics (const std::list <ProtocolStatistics> & protocol_statistics)
{
    m_view->set_filtered_protocol_statistics (protocol_statistics);
}
void CCtrl::set_filtered_frenquency (const std::list <HostFrenquency> & host_frenquency)
{
    m_view->set_filtered_frenquency (host_frenquency);
}
void CCtrl::set_filtered_flow_pie_data (const std::list <FlowPieData> & flow_pie_data)
{
    m_view->set_filtered_flow_pie_data (flow_pie_data);
}

void  CCtrl::set_basic_info (const BasicInfo & basic_info)
{
    m_view->set_basic_info (basic_info);
}
void CCtrl::set_filter (const std::list <IpFilterInfo> & ip_filter_info)
{
    m_model->set_filter (ip_filter_info);
}
 void CCtrl::set_wave_statistics (const std::list <WaveData> & wave_statistics)
{
    m_view->set_wave_statistics(wave_statistics);
}
 void CCtrl::set_filtered_wave_statistics (const std::list <WaveData> & wave_statistics)
{
    m_view->set_filtered_wave_statistics(wave_statistics);
}
  void CCtrl::set_report_statistics (const ReportStatInfo & report_stat_info)
 {
     m_view->set_report_statistics(report_stat_info);
 }
   bool CCtrl::save_filter_record_to_file(const std::vector<FilterRecord> & v_filter_record)
  {
      return m_model->save_filter_record_to_file(v_filter_record);
  }
   bool CCtrl::get_filter_record_from_file (std::vector<FilterRecord> & v_filter_record)
  {
      std::cout << "CCtrl ::get_filter_record_from_file\n";
      return m_model->get_filter_record_from_file(v_filter_record);
  }
void CCtrl::grab_pie_wave()
{
         m_view->grab_pie_wave();
}