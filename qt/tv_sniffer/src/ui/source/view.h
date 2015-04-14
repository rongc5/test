/** 
 * Copyright (C) 2007-2013 Haier
 *
 * @file             view.h
 * @brief            view 实现
 * @author           zhengyl
 * @version	         1.0
 * @date	         2014-1-24
 */
#ifndef  _VIEW_H_
#define  _VIEW_H_

class CPacketListModel;
struct  Packet_Record;
class CHistogram_Widget;
class CPacketListView;
class QWidget;
class CReportWidget;

class CView : public IView
{
public:
    CView ();
    /** 初始化
     * 
     * @param  ctrl
     * @return void
     * @see 
     * @note 
     */
    virtual void init (ICtrl * ctrl);
    /** 设置widget
     * 
     * @param  widget
     * @return void
     * @see 
     * @note 
     */
    virtual void set_widget (QWidget * widget);
    /** 清空packet list
     * 
     * @return 
     * @see 
     * @note 
     */
    virtual void clear_packet_list ();
    /** 添加packet
     * 
     * @param  packet_record
     * @return void
     * @see 
     * @note 
     */
    virtual void add_packet (const Packet_Record & packet_record);
    /** 删除自身内存
     * 
     * @return 
     * @see 
     * @note 
     */
    virtual void release ();
    /** 开始捕获
     * 
     * @return 
     * @see 
     * @note 
     */
    virtual void start_capture ();
    /** 停止捕获
     * 
     * @return 
     * @see 
     * @note 
     */
    virtual void stop_capture ();
    /** 更新
     * 
     * @return 
     * @see 
     * @note 
     */
    virtual void update ();
    /** 添加包数
     * 
     * @param  packet_count 包数
     * @return 
     * @see 
     * @note 
     */
    virtual void add_packet_count (int packet_count);

   /** 设置ip统计
    * 
    * @param  ip_statistics ip统计列表
    * @return 
    * @see 
    * @note 
    */
   virtual void set_ip_statistics (const std::list <IpStatisticsRecord> & ip_statistics);

    /** 设置协议统计
     * 
     * @param  protocol_statistics 协议统计列表
     * @return 
     * @see 
     * @note 
     */
    virtual void 
        set_protocol_statistics (const std::list <ProtocolStatistics> & protocol_statistics) ;
    /** 设置频率统计
     * 
     * @param  host_frenquency 频率统计列表
     * @return 
     * @see 
     * @note 
     */
    virtual void 
        set_frenquency (const std::list <HostFrenquency> & host_frenquency) ;
    /** 设置流量饼图
     * 
     * @param  flow_pie_data 流量饼图数据列表
     * @return 
     * @see 
     * @note 
     */
    virtual void 
        set_flow_pie_data (const std::list <FlowPieData> & flow_pie_data) ;

    /** 设置被过滤的ip统计
     * 
     * @param  ip_statistics ip统计列表
     * @return 
     * @see 
     * @note 
     */
    virtual void set_filtered_ip_statistics (const std::list <IpStatisticsRecord> & ip_statistics);
    /** 设置被过滤的协议统计
     * 
     * @param  protocol_statistics 协议统计列表
     * @return 
     * @see 
     * @note 
     */
    virtual void 
        set_filtered_protocol_statistics (const std::list <ProtocolStatistics> & protocol_statistics) ;
    /** 设置被过滤的频率统计
     * 
     * @param  host_frenquency  主机频率列表
     * @return 
     * @see 
     * @note 
     */
    virtual void 
        set_filtered_frenquency (const std::list <HostFrenquency> & host_frenquency) ;
    /** 设置被过滤的流量饼图数据
     * 
     * @param  flow_pie_data  流量饼图数据列表
     * @return 
     * @see 
     * @note 
     */
    virtual void 
        set_filtered_flow_pie_data (const std::list <FlowPieData> & flow_pie_data) ;

    /** 设置基本信息
     * 
     * @param  basic_info   基本信息
     * @return 
     * @see 
     * @note 
     */
    virtual void set_basic_info (const BasicInfo & basic_info) ;

    /** 设置过滤器
     * 
     * @param  ip_filter_info  过滤器信息
     * @return 
     * @see 
     * @note 
     */
    virtual void set_filter (const std::list <IpFilterInfo> & ip_filter_info);

    /** 设置波形统计
     * 
     * @param  wave_statistics  波形统计
     * @return void
     * @see 
     * @note 
     */
    virtual void set_wave_statistics (const std::list <WaveData> & wave_statistics);
    /** 设置被过滤的波形统计
     * 
     * @param  wave_statistics 波形统计
     * @return 
     * @see 
     * @note 
     */
    virtual void set_filtered_wave_statistics (const std::list <WaveData> & wave_statistics);

    virtual void set_report_statistics (const ReportStatInfo & report_stat_info);
    virtual bool save_filter_record_to_file(const std::vector<FilterRecord> & v_filter_record) ;
    virtual bool get_filter_record_from_file (std::vector<FilterRecord> & v_filter_record) ;

    virtual CReportWidget *get_report_widget();
    virtual void grab_pie_wave();
private:
    ICtrl *  m_ctrl;
    CPacketListModel * m_packet_list_model;
    CHistogram_Widget * m_histogram_widget;
    CPacketListView * m_packet_list_view;
    CReportWidget   * m_report_widget;
    QWidget *         m_main_win;
};


#endif
