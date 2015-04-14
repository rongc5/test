/** 
 * Copyright (C) 2007-2013 Haier
 *
 * @file             model.h
 * @brief            model实现
 * @author           zhengyl
 * @version	         1.0
 * @date	         2014-1-24
 */
#ifndef _MODEL_H_
#define _MODEL_H_

class ICtrl;
class QThread;
class CMessageReceiver;
class CMDSender;
class QUdpSocket;
class CPacketsCache;
class CPacketsSender;
class CStatisticsThread;
class CIPFilter;
class QProcess;
class BaseThread;
class QMutex;
class QTextDocument;
class CFilterFile;

class CModel : public IModel
{
public:
    CModel ();
    ~CModel ();
    /** 初始化
     * 
     * @param  ctrl 控制指针
     * @return 
     * @see 
     * @note 
     */
    virtual void init (ICtrl * ctrl);
    /** 运行
     * 
     * @return void
     * @see 
     * @note 
     */
    virtual void run ();
    /** 开始捕获
     * 
     * @return void
     * @see 
     * @note 
     */
    virtual void start_capture ();
    virtual void start_capture_impl ();
    /** 停止捕获
     * 
     * @return void
     * @see 
     * @note 
     */
    virtual void stop_capture ();
    /** 增加数据包
     * 
     * @param  packet_record 数据包
     * @return void
     * @see 
     * @note 
     */
    virtual void add_packet (const Packet_Record & packet_record);
    /** 增加包数
     * 
     * @param  count 包数
     * @return void
     * @see 
     * @note 
     */
    virtual void add_packet_count (int count);

    /** 设置ip统计
     * 
     * @param  ip_statistics ip统计数据列表
     * @return void
     * @see 
     * @note 
     */
    virtual void set_ip_statistics (const std::list <IpStatisticsRecord> & ip_statistics);
    /** 设置协议统计
     * 
     * @param  protocol_statistics 协议统计列表
     * @return  void
     * @see 
     * @note 
     */
    virtual void 
        set_protocol_statistics (const std::list <ProtocolStatistics> & protocol_statistics);
    /** 设置频率
     * 
     * @param  host_frenquency 频率统计数据列表
     * @return void
     * @see 
     * @note 
     */
    virtual void 
        set_frenquency (const std::list <HostFrenquency> & host_frenquency);
    /** 设置流量数据
     * 
     * @param  host_frenquency 流量饼图数据列表
     * @return 
     * @see 
     * @note 
     */
    virtual void 
        set_flow_pie_data (const std::list <FlowPieData> & host_frenquency);

    /** 设置被过滤的ip统计
     * 
     * @param  ip_statistics ip统计数据列表
     * @return void
     * @see 
     * @note 
     */
    virtual void set_filtered_ip_statistics (const std::list <IpStatisticsRecord> & ip_statistics);
    /** 设置被过滤的协议统计
     * 
     * @param  protocol_statistics 协议统计数据列表
     * @return void
     * @see 
     * @note 
     */
    virtual void 
        set_filtered_protocol_statistics (const std::list <ProtocolStatistics> & protocol_statistics);
    /** 设置被过滤的频率统计
     * 
     * @param  host_frenquency 频率数据统计
     * @return void
     * @see 
     * @note 
     */
    virtual void 
        set_filtered_frenquency (const std::list <HostFrenquency> & host_frenquency);
    /** 设置被过滤的流量饼图数据
     * 
     * @param  flow_pie_data 流量饼图数据
     * @return void
     * @see 
     * @note 
     */
    virtual void 
        set_filtered_flow_pie_data (const std::list <FlowPieData> & flow_pie_data);

    /** 设置基本信息
     * 
     * @param  basic_info  基本信息
     * @return void
     * @see 
     * @note 
     */
    virtual void  set_basic_info (const BasicInfo & basic_info);

    /** 设置过滤器
     * 
     * @param  ip_filter_info 过滤器信息
     * @return void
     * @see 
     * @note 
     */
    virtual void set_filter (const std::list <IpFilterInfo> & ip_filter_info);

    /** 设置波形统计
     * 
     * @param  wave_statistics 波形统计信息
     * @return void
     * @see 
     * @note 
     */
    virtual void set_wave_statistics (const std::list <WaveData> & wave_statistics);
    /** 设置被过滤的波形统计信息
     * 
     * @param  wave_statistics 波形统计数据列表
     * @return  void
     * @see 
     * @note 
     */
    virtual void set_filtered_wave_statistics (const std::list <WaveData> & wave_statistics);

    virtual void set_report_statistics (const ReportStatInfo & report_stat_info);
    virtual bool save_filter_record_to_file(const std::vector<FilterRecord> & v_filter_record);
    virtual bool get_filter_record_from_file (std::vector<FilterRecord> & v_filter_record);

   


protected:
private:
		void send_report();
	
    void construct_report_document (QTextDocument & document,
		    const BasicInfo & basic_info,
		    const ReportStatInfo & report_stat_info,
		    const std::string & wave_file,
		    const std::string & filtered_wave_file,
		    const std::string & pie_file,
		    const std::string & filtered_pie_file);


    int append_basic_info (QTextCursor & cursor, 
            const BasicInfo & basic_info);
//    int append_protocol_statistic_info (QTextCursor & cursor, 
//        const std::list <ProtocolStatistics> & protocol_statistics);
        	
         int append_protocol_statistic_info (QTextDocument * doc, 
        const std::list <ProtocolStatistics> & protocol_statistics);

    int append_wave_statistic_info (QTextDocument * doc, const std::string & file);

    int append_flow_pie_statistic_info (QTextCursor & cursor, const std::string & file);
        	
    int append_frenquency_statistic_info (QTextDocument * doc, 
        const std::list <HostFrenquency> & frenquency_statistics);
    
    int append_ip_statistic_info (QTextDocument * doc, 
        const std::list <IpStatisticsRecord> & ip_statistics);
   
        	  int append_dns_statistic_info (QTextDocument * doc, 
        const std::list <DnsInfo> & frenquency_statistics);

private:
    ICtrl * m_ctrl;
    QThread * m_test_thread;
    CMDSender * m_cmd_sender;
    CMessageReceiver * m_message_receiver;
    QUdpSocket * m_udp_socket;
    CPacketsCache  * m_packets_cache;
    CPacketsSender * m_packets_sender;
    CStatisticsThread * m_statistics_thread;
    CIPFilter      *    m_ip_filter;
    QProcess *         m_process;
    BaseThread  * m_base;
    QDateTime start;
    QDateTime end;
    QMutex   m_start_stop_mutex;
    CFilterFile * m_filter_file;
    CStartThread *  m_start_thread;
    std::list <CStartThread *> m_v_start_thread;
};



#endif
