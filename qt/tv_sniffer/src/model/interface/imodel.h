
/** 
 * Copyright (C) 2007-2013 Haier
 *
 * @file             imodel.h
 * @brief            model接口
 * @author           zhengyl
 * @version	         1.0
 * @date	         2014-1-24
 */
#ifndef _IMODEL_H_
#define _IMODEL_H_
#include <vector>
class IView;
class ICtrl;
struct Packet_Record;
struct ProtocolStatistics;
struct HostFrenquency;
struct FlowPieData;
struct BasicInfo;
struct IpFilterInfo;
struct IpStatisticsRecord;
struct WaveData;
struct ReportStatInfo;
class QTextDocument;
struct FilterRecord;

class IModel
{
public:
    /** 初始化
     * 
     * @param  ctrl
     * @return 
     * @see 
     * @note 
     */
    virtual void init (ICtrl * ctrl) = 0;
    /** 运行
     * 
     * @return 
     * @see 
     * @note 
     */
    virtual void run () = 0;
    /** 开始抓包(异步启动)
     * 
     * @return 
     * @see 
     * @note 
     */
    virtual void start_capture () = 0;
    /** 开始抓包（同步启动）
     * 
     * @return 
     * @see 
     * @note 
     */
    virtual void start_capture_impl () = 0;
    /** 停止抓包
     * 
     * @return 
     * @see 
     * @note 
     */
    virtual void stop_capture () = 0;
    /** 添加包
     * 
     * @param  packet_record
     * @return 
     * @see 
     * @note 
     */
    virtual void add_packet (const Packet_Record & packet_record) = 0;
    /** 添加包数
     * 
     * @param  count
     * @return 
     * @see 
     * @note 
     */
    virtual void add_packet_count (int count) = 0;

    /** 设置ip统计
     * 
     * @param  ip_statistics
     * @return 
     * @see 
     * @note 
     */
    virtual void set_ip_statistics (const std::list <IpStatisticsRecord> & ip_statistics) = 0;
    /** 设置协议统计
     * 
     * @param  protocol_statistics
     * @return 
     * @see 
     * @note 
     */
    virtual void 
        set_protocol_statistics (const std::list <ProtocolStatistics> & protocol_statistics) = 0;
    /** 设置频率
     * 
     * @param  host_frenquency
     * @return 
     * @see 
     * @note 
     */
    virtual void 
        set_frenquency (const std::list <HostFrenquency> & host_frenquency) = 0;
    /** 设置流量饼图数据
     * 
     * @param  host_frenquency
     * @return 
     * @see 
     * @note 
     */
    virtual void 
        set_flow_pie_data (const std::list <FlowPieData> & host_frenquency) = 0;

    /** 设置过滤后的ip统计
     * 
     * @param  ip_statistics
     * @return 
     * @see 
     * @note 
     */
    virtual void set_filtered_ip_statistics (const std::list <IpStatisticsRecord> & ip_statistics) = 0;
    /** 设置过滤后的协议统计
     * 
     * @param  protocol_statistics
     * @return 
     * @see 
     * @note 
     */
    virtual void 
        set_filtered_protocol_statistics (const std::list <ProtocolStatistics> & protocol_statistics) = 0;
    /** 设置过滤后的频率统计
     * 
     * @param  host_frenquency
     * @return 
     * @see 
     * @note 
     */
    virtual void 
        set_filtered_frenquency (const std::list <HostFrenquency> & host_frenquency) = 0;
    /** 设置过滤后的流量饼图数据
     * 
     * @param  flow_pie_data
     * @return 
     * @see 
     * @note 
     */
    virtual void 
        set_filtered_flow_pie_data (const std::list <FlowPieData> & flow_pie_data) = 0;

    /** 设置基本信息
     * 
     * @param  basic_info
     * @return 
     * @see 
     * @note 
     */
    virtual void  set_basic_info (const BasicInfo & basic_info) = 0;

    /** 设置过滤器
     * 
     * @param  ip_filter_info
     * @return 
     * @see 
     * @note 
     */
    virtual void set_filter (const std::list <IpFilterInfo> & ip_filter_info) = 0;

    /** 设置波形统计信息
     * 
     * @param  wave_statistics
     * @return 
     * @see 
     * @note 
     */
    virtual void set_wave_statistics (const std::list <WaveData> & wave_statistics) = 0;
    /** 设置被过滤的波形统计信息
     * 
     * @param  wave_statistics
     * @return 
     * @see 
     * @note 
     */
    virtual void set_filtered_wave_statistics (const std::list <WaveData> & wave_statistics) = 0;

    virtual void set_report_statistics (const ReportStatInfo & report_stat_info) = 0;

    virtual bool save_filter_record_to_file(const std::vector<FilterRecord> & v_filter_record) = 0;
    virtual bool get_filter_record_from_file (std::vector<FilterRecord> & v_filter_record) = 0;


    virtual ~IModel () {} 
};

/** 获取model
 * 
 * @return 
 * @see 
 * @note 
 */
IModel * get_model ();

#endif