
/** 
 * Copyright (C) 2007-2013 Haier
 *
 * @file             report_widget.h
 * @brief            report wlg
 * @author           zhengyl
 * @version	         1.0
 * @date	         2014-1-24
 */
#ifndef _REPORT_WIDGET_H_
#define _REPORT_WIDGET_H_
#include "../forms/ui_dir/ui_report.h"
class QWidget;
class CProtocolStatisticModel;
class CIpFrenqModel;
class CIpStatisticsModel;
class CPieChartWidget;
class QCustomPlot;
class CDnsInfoModel;

class  CReportWidget : public QWidget
{
public:
    CReportWidget ();
    /** 清空
     * 
     * @return void
     * @see 
     * @note 
     */
    void clear ();
    /** 设置ip统计
     * 
     * @param  ip_statistics ip统计列表
     * @return void
     * @see 
     * @note 
     */
    void set_ip_statistics (const std::list <IpStatisticsRecord> & ip_statistics);
    /** 设置协议统计
     * 
     * @param  protocol_statistics 协议统计列表
     * @return void
     * @see 
     * @note 
     */
    void set_protocol_statistics (const std::list <ProtocolStatistics> & protocol_statistics) ;
    /** 设置频率
     * 
     * @param  host_frenquency 频率列表
     * @return 
     * @see 
     * @note 
     */
    void set_frenquency (const std::list <HostFrenquency> & host_frenquency);
    /** 设置流量饼图
     * 
     * @param  flow_pie_data 流量饼图列表
     * @return 
     * @see 
     * @note 
     */
    void set_flow_pie_data (const std::list <FlowPieData> & flow_pie_data);

    /** 设置被过滤的ip统计
     * 
     * @param  ip_statistics ip统计列表
     * @return 
     * @see 
     * @note 
     */
    void set_filtered_ip_statistics (const std::list <IpStatisticsRecord> & ip_statistics);
    /** 设置被过滤的协议统计
     * 
     * @param  protocol_statistics 协议统计列表
     * @return 
     * @see 
     * @note 
     */
    void set_filtered_protocol_statistics (const std::list <ProtocolStatistics> & protocol_statistics);
    /** 设置被过滤的频率
     * 
     * @param  host_frenquency 频率列表
     * @return 
     * @see 
     * @note 
     */
    void set_filtered_frenquency (const std::list <HostFrenquency> & host_frenquency);
    /** 设置被过滤的流量饼图数据
     * 
     * @param  flow_pie_data 流量饼图数据列表
     * @return void
     * @see 
     * @note 
     */
    void set_filtered_flow_pie_data (const std::list <FlowPieData> & flow_pie_data);
    /** 设置基本信息
     * 
     * @param  basic_info 基本信息
     * @return 
     * @see 
     * @note 
     */
    void set_basic_info (const BasicInfo & basic_info);
    /** 设置波形统计信息
     * 
     * @param  wave_statistics  波形统计数据列表
     * @return 
     * @see 
     * @note 
     */
    void set_wave_statistics (const std::list <WaveData> & wave_statistics, const QDateTime & start, const QDateTime & end);
    /** 设置被过滤的波形统计信息
     * 
     * @param  wave_statistics  波形统计列表
     * @return void
     * @see 
     * @note 
     */
    void set_filtered_wave_statistics (const std::list <WaveData> & wave_statistics, const QDateTime & start, const QDateTime & end);
    void set_dns_statistics (const std::list <DnsInfo> & dns_statistics);
    void set_filtered_dns_statistics (const std::list <DnsInfo> & dns_statistics);

    void set_report_statistics (const ReportStatInfo & report_stat_info);

    Ui_report *get_ui_report(); 
        void grab_pip_and_wave();

protected:
private:
    Ui_report m_ui_report;
private:
    CProtocolStatisticModel * m_protocol_statistics_model;
    CProtocolStatisticModel * m_filtered_protocol_statistics_model;
    CIpFrenqModel *           m_ip_frenq_model;
    CIpFrenqModel *           m_filtered_ip_frenq_model;
    CIpStatisticsModel *      m_ip_statistic_model;
    CIpStatisticsModel *      m_filtered_ip_statistic_model;
    CPieChartWidget *         m_pie_char_wlg;
    CPieChartWidget *         m_filtered_pie_char_wlg;
    QCustomPlot    *          m_custom_plot;
    QCustomPlot    *          m_filtered_custom_plot;
    CDnsInfoModel  *          m_dns_info_model;
    CDnsInfoModel  *          m_filtered_dns_info_model;

    CPieChartWidget *         m_pie_char_popup_wlg;
    CPieChartWidget *         m_filtered_pie_char_popup_wlg;

    QCustomPlot    *          m_custom_plot_popup;
    QCustomPlot    *          m_filtered_custom_plot_popup;

};


#endif
