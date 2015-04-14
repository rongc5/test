
/** 
 * Copyright (C) 2007-2013 Haier
 *
 * @file             iview.h
 * @brief            view接口类
 * @author           zhengyl
 * @version	         1.0
 * @date	         2014-1-24
 */
#ifndef _IVIEW_H_
#define _IVIEW_H_
#include <list>
#include <vector>

struct Packet_Record;
class IModel;
class ICtrl;
struct ProtocolStatistics;
struct HostFrenquency;
struct FlowPieData;
struct BasicInfo;
struct IpFilterInfo;
struct IpStatisticsRecord;
class QWidget;
struct WaveData;
struct ReportStatInfo;
struct CReportWidget;
class FilterRecord;

class IView
{
public :
    /** 初始化
     * 
     * @param  ctrl 控制指针
     * @return 
     * @see 
     * @note 
     */
    virtual void init ( ICtrl * ctrl) = 0;
    /** 设置widget
     * 
     * @param  widget 窗口指针
     * @return void
     * @see 
     * @note 
     */
    virtual void set_widget (QWidget * widget) = 0;
    /** 清空packet list
     * 
     * @return void
     * @see 
     * @note 
     */
    virtual void clear_packet_list () = 0;
    /** 增加包数据
     * 
     * @param  packet_record 包数据
     * @return void
     * @see 
     * @note 
     */
    virtual void add_packet (const Packet_Record & packet_record) = 0;
    /** 开始捕获
     * 
     * @return void
     * @see 
     * @note 
     */
    virtual void start_capture () = 0;
    /** 停止捕获
     * 
     * @return void
     * @see 
     * @note 
     */
    virtual void stop_capture () = 0;
    /** 释放自身内存
     * 
     * @return void
     * @see 
     * @note 
     */
    virtual void release () = 0;
    /** 更新
     * 
     * @return void
     * @see 
     * @note 
     */
    virtual void update () = 0;
    /** 增加包数
     * 
     * @param  packet_count 包数
     * @return void
     * @see 
     * @note 
     */
    virtual void add_packet_count (int packet_count) = 0;

    //报告数据
    /** 设置ip统计
     * 
     * @param  ip_statistics ip统计信息
     * @return  void
     * @see 
     * @note 
     */
    virtual void set_ip_statistics (const std::list <IpStatisticsRecord> & ip_statistics) = 0;

    /** 设置协议统计
     * 
     * @param  protocol_statistics 协议统计信息
     * @return void
     * @see 
     * @note 
     */
    virtual void 
        set_protocol_statistics (const std::list <ProtocolStatistics> & protocol_statistics) = 0;
    /** 设置频率统计
     * 
     * @param  host_frenquency 频率统计信息
     * @return void
     * @see 
     * @note 
     */
    virtual void 
        set_frenquency (const std::list <HostFrenquency> & host_frenquency) = 0;
    /** 设置流量饼图数据
     * 
     * @param  flow_pie_data 流量饼图数据
     * @return void
     * @see 
     * @note 
     */
    virtual void 
        set_flow_pie_data (const std::list <FlowPieData> & flow_pie_data) = 0;

    /** 设置被过滤的ip统计
     * 
     * @param  ip_statistics ip统计列表
     * @return void
     * @see 
     * @note 
     */
    virtual void set_filtered_ip_statistics (const std::list <IpStatisticsRecord> & ip_statistics) = 0;
    /** 设置别过滤的协议统计
     * 
     * @param  protocol_statistics 协议统计列表
     * @return void
     * @see 
     * @note 
     */
    virtual void 
        set_filtered_protocol_statistics (const std::list <ProtocolStatistics> & protocol_statistics) = 0;
    /** 设置被过滤的频率统计
     * 
     * @param  host_frenquency 频率统计列表
     * @return void
     * @see 
     * @note 
     */
    virtual void 
        set_filtered_frenquency (const std::list <HostFrenquency> & host_frenquency) = 0;
    /** 设置被过滤的流量饼图数据
     * 
     * @param  flow_pie_data 流量饼图列表
     * @return void
     * @see 
     * @note 
     */
    virtual void 
        set_filtered_flow_pie_data (const std::list <FlowPieData> & flow_pie_data) = 0;

    /** 设置基本信息
     * 
     * @param  basic_info 基本信息
     * @return void
     * @see 
     * @note 
     */
    virtual void set_basic_info (const BasicInfo & basic_info) = 0;

    /** 设置过滤器
     * 
     * @param  ip_filter_info 过滤器信息
     * @return void
     * @see 
     * @note 
     */
    virtual void set_filter (const std::list <IpFilterInfo> & ip_filter_info) = 0;

    /** 设置波形统计
     * 
     * @param  wave_statistics 波形统计列表
     * @return void
     * @see 
     * @note 
     */
    virtual void set_wave_statistics (const std::list <WaveData> & wave_statistics) = 0;
    /** 设置被过滤的波形统计信息
     * 
     * @param  wave_statistics 波形统计列表
     * @return void
     * @see 
     * @note 
     */
    virtual void set_filtered_wave_statistics (const std::list <WaveData> & wave_statistics) = 0;

    virtual void set_report_statistics (const ReportStatInfo & report_stat_info) = 0;

    virtual CReportWidget *get_report_widget() = 0;
    virtual void grab_pie_wave() = 0;
    virtual bool save_filter_record_to_file(const std::vector<FilterRecord> & v_filter_record) = 0;
    virtual bool get_filter_record_from_file (std::vector<FilterRecord> & v_filter_record) = 0;

    virtual ~IView (){}
};

IView * get_view ();


#endif