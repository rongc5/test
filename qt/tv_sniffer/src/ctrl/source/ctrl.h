/** 
 * Copyright (C) 2007-2013 Haier
 *
 * @file             ctrl.h
 * @brief            控制器
 * @author           zhengyl
 * @version	         1.0
 * @date	         2014-1-24
 */
#ifndef  _CTRL_H_
#define  _CTRL_H_
class ICtrl;
class IModel;
class IView;


class CCtrl : public ICtrl
{
public:
    CCtrl ();
    /** 初始化
     * 
     * @param  model
     * @param  view
     * @return 
     * @see 
     * @note 
     */
    virtual void init (IModel * model, IView * view);
    /** 开始抓包
     * 
     * @return 
     * @see 
     * @note 
     */
    virtual void start_capture ();
    /** 停止抓包
     * 
     * @return 
     * @see 
     * @note 
     */
    virtual void stop_capture ();
    /** 清除包列表
     * 
     * @return 
     * @see 
     * @note 
     */
    virtual void clear_packet_list ();
    /** 添加包
     * 
     * @param  packet_record
     * @return 
     * @see 
     * @note 
     */
    virtual void add_packet (const Packet_Record & packet_record);
    /** 更新显示
     * 
     * @return 
     * @see 
     * @note 
     */
    virtual void update_view ();
    /** 增加包数显示
     * 
     * @param  packet_count
     * @return 
     * @see 
     * @note 
     */
    virtual void add_packet_count (int packet_count);

    /** 设置ip统计
     * 
     * @param  ip_statistics
     * @return 
     * @see 
     * @note 
     */
    virtual void set_ip_statistics (const std::list <IpStatisticsRecord> & ip_statistics);
    /** 设置协议统计
     * 
     * @param  protocol_statistics
     * @return 
     * @see 
     * @note 
     */
    virtual void 
        set_protocol_statistics (const std::list <ProtocolStatistics> & protocol_statistics);
    /** 设置频率统计
     * 
     * @param  host_frenquency
     * @return 
     * @see 
     * @note 
     */
    virtual void 
        set_frenquency (const std::list <HostFrenquency> & host_frenquency);
    /** 设置饼图数据
     * 
     * @param  host_frenquency
     * @return 
     * @see 
     * @note 
     */
    virtual void 
        set_flow_pie_data (const std::list <FlowPieData> & host_frenquency);

    /** 设置过滤的ip统计
     * 
     * @param  ip_statistics
     * @return 
     * @see 
     * @note 
     */
    virtual void set_filtered_ip_statistics (const std::list <IpStatisticsRecord> & ip_statistics);
    /** 设置过滤的协议统计
     * 
     * @param  protocol_statistics
     * @return 
     * @see 
     * @note 
     */
    virtual void 
        set_filtered_protocol_statistics (const std::list <ProtocolStatistics> & protocol_statistics);
    /** 设置过滤的频率显示
     * 
     * @param  host_frenquency
     * @return 
     * @see 
     * @note 
     */
    virtual void 
        set_filtered_frenquency (const std::list <HostFrenquency> & host_frenquency);
    /** 设置过滤的流量饼图显示
     * 
     * @param  flow_pie_data
     * @return 
     * @see 
     * @note 
     */
    virtual void 
        set_filtered_flow_pie_data (const std::list <FlowPieData> & flow_pie_data);

    /** 设置基本信息
     * 
     * @param  basic_info
     * @return 
     * @see 
     * @note 
     */
    virtual void  set_basic_info (const BasicInfo & basic_info);

    /** 设置过滤器信息
     * 
     * @param  ip_filter_info
     * @return 
     * @see 
     * @note 
     */
    virtual void set_filter (const std::list <IpFilterInfo> & ip_filter_info);

    /** 设置波形统计
     * 
     * @param  wave_statistics
     * @return 
     * @see 
     * @note 
     */
    virtual void set_wave_statistics (const std::list <WaveData> & wave_statistics);
    /** 设置被过滤的波形统计
     * 
     * @param  wave_statistics
     * @return 
     * @see 
     * @note 
     */
    virtual void set_filtered_wave_statistics (const std::list <WaveData> & wave_statistics);

    virtual void set_report_statistics (const ReportStatInfo & report_stat_info);
    virtual void grab_pie_wave();
    virtual bool save_filter_record_to_file(const std::vector<FilterRecord> & v_filter_record);
    virtual bool get_filter_record_from_file (std::vector<FilterRecord> & v_filter_record);
protected:
private:
    IModel * m_model;
    IView *  m_view;
};


#endif