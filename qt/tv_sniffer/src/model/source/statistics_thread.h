/**
 * @file statistics_thread.h
 * @brief tvsniffer统计代码，头文件部分
 *		  统计分为两部分 1：全量统计部分 2：过滤统计部分
 *		  统计相关子项包含五部分
 *		  1：IP统计
 *		  2：协议统计
 *		  3：IP-次数-大小统计
 *		  4：IP访问频次统计(每五分钟)
 *		  5：IP访问频次统计(每分钟)
 * @author yangl
 * @version 1.0.0
 * @date 2014-02-11
 */

#ifndef _STATICS_THREAD_H_
#define _STATICS_THREAD_H_
#include <list>
#include <QThread>
#include <QTimerEvent>
#include <QMutexLocker>
#include <QWaitCondition>
#include <QMutex>
#include <map>


/**
 * @brief Packet_Record 原始数据包结构体
 */
struct Packet_Record;
/**
 * @brief CIPFilter 过滤数据包结构
 */
class  CIPFilter;
/**
 * @brief FlowPieData 统计IP(IP,包数)数据结构
 */
struct FlowPieData;
/**
 * @brief ProtocolStatistics 统计协议数据结构
 */
struct ProtocolStatistics;
/**
 * @brief IpStatisticsRecord 统计IP（IP，包数，大小）数据结构
 */
struct IpStatisticsRecord;
/**
 * @brief HostFrenquency 统计IP(IP,包数)访问频次(每五分钟) 数据结构 
 */
struct HostFrenquency;
/**
 * @brief WaveData 统计IP(IP,包数)访问频次(每分钟) 数据结构
 */
struct WaveData;

struct ReportStatInfo;

struct DnsInfo;

class CStatisticsThread : public QThread
{
public:
    CStatisticsThread (CIPFilter * filter);
 /**
 * @brief 获取原始数据接口
 *
 * @param packet_record 原始数据包结构体
 *
 * @return void
 */
	void add_packet (const Packet_Record & packet_record);

 /**
 * @brief 抓取总包数统计对外接口
 *
 * @param count 总包数记录变量
 *
 * @return 
 */

    void get_packet_count (int &count);

	/**
	 * @brief 全量统计部分
	 */

    void get_report_statistics (ReportStatInfo & report_info);
 

protected:
    virtual void run ();
    void timerEvent(QTimerEvent *event);
private:

 /**
 * @brief 处理原始数据的接口
 *
 * @param record 原始数据包结构
 *
 * @return 
 */
	void process_packets (const struct Packet_Record & record);

 /**
 * @brief 获取IP统计数据(IP与次数)统计数据对外接口
 *
 * @param host_frenquency IP统计数据结构
 *
 * @return void
 */
    void get_flow_pie_data (std::list <FlowPieData> & host_frenquency);
 /**
 * @brief 获取协议统计数据(协议与次数)统计对外接口
 *
 * @param protocol_statistics 协议统计数据结构
 *
 * @return 
 */
    void get_protocol_statistics (std::list <ProtocolStatistics> & protocol_statistics);
 /**
 * @brief 获取IP统计数据(IP,次数与total size)对外接口
 *
 * @param ip_statistics IP统计（IP，包数，大小）数据结构
 *
 * @return void
 */
    void get_ip_statistics (std::list <IpStatisticsRecord> & ip_statistics);
 /**
 * @brief 获取IP访问频次(每五分钟) 对外接口
 *
 * @param host_frenquency IP访问频次数据结构
 *
 * @return void
 */
    void get_frenquency (std::list <HostFrenquency> & host_frenquency);
 /**
 * @brief 获取IP频次(每分钟) 对外接口
 *
 * @param wave_statistics IP访问频次数据结构
 *
 * @return 
 */
    void get_wave_statistics (std::list <WaveData> & wave_statistics);

	/**
	 * @brief 过滤统计部分 
	 */

 /**
 * @brief 获取过滤后的IP统计数据（IP，次数与大小） 对外接口
 *
 * @param ip_statistics IP统计（IP，包数，大小）数据结构
 *
 * @return 
 */
    void get_filtered_ip_statistics (std::list <IpStatisticsRecord> & ip_statistics);
 /**
 * @brief  获取过滤后的协议统计数据对外接口
 *
 * @param protocol_statistics 协议统计数据结构
 *
 * @return 
 */
    void get_filtered_protocol_statistics (std::list <ProtocolStatistics> & protocol_statistics);
 /**
 * @brief 获取过滤后的IP统计数据(IP,包数)对外接口
 *
 * @param flow_pie_data IP统计数据结构
 *
 * @return 
 */
    void get_filtered_flow_pie_data (std::list <FlowPieData> & flow_pie_data);
 /**
 * @brief 获取过滤后的IP访问频次(每五分钟) 对外接口
 *
 * @param host_frenquency IP访问频次数据结构
 *
 * @return 
 */
    void get_filtered_frenquency (std::list <HostFrenquency> & host_frenquency);


 /**
 * @brief 获取过滤后的IP频次(每分钟) 对外接口
 *
 * @param wave_statistics IP访问频次数据结构
 *
 * @return 
 */
    void get_filtered_wave_statistics (std::list <WaveData> & wave_statistics);
/**
 * @brief 域名统计（域名与IP）
 *
 * @param dnsip_stat 域名统计临时数据结构
 */
    void get_dns_info (std::list <DnsInfo> & dns_info);
/**
 * @brief 域名统计（域名与IP）
 *
 * @param dnsip_stat  过滤后的域名统计临时数据结构
 */
    void get_filtered_dns_info (std::list <DnsInfo> & dns_info);
 /**
 * @brief 统计IP的临时函数,主要用于每分钟IP访问频次
 *
 * @param record 原始数据包结构
 * @param Counts_map IP统计数据结构（定次清除统计数据）
 *
 * @return 
 */

void temp_ip_One(const struct Packet_Record & record,std::map<std::string,int> *Counts_map);
 /**
 * @brief IP统计临时函数
 *
 * @param record 原始数据包结构
 * @param I_map IP统计数据结构
 *
 * @return 
 */
void Temp_get_flow_pie_data(const struct Packet_Record & record,std::map<std::string,int> *I_map);
 /**
 * @brief 协议统计临时函数
 *
 * @param record 原始数据包结构
 * @param P_map 协议统计数据结构
 *
 * @return 
 */
void Temp_get_protocol_statistics(const struct Packet_Record & record,std::map<std::string,int> *P_map);
 /**
 * @brief IP统计临时函数（IP.次数，大小）
 *
 * @param record 原始数据结构
 * @param m_IpStatisticsRecord IP统计（IP，包数，大小）数据结构 
 *
 * @return 
 */
void Temp_get_ip_statistics(const struct Packet_Record & record,std::list <IpStatisticsRecord> *m_IpStatisticsRecord);
 /**
 * @brief 每五分钟统计IP访问频次 临时函数
 *
 * @param I_map IP统计数据结构
 * @param m_frenquency IP统计频次数据结构
 *
 * @return 
 */
void Temp_get_frenquency(std::map<std::string,int> *I_map,std::map<std::string,std::list <int> > *m_frenquency);
 /**
 * @brief 每分钟访问频次统计 临时函数
 *
 * @param I_map IP统计数据结构
 * @param m_count IP统计频次数据结构
 *
 * @return 
 */
void Temp_set_wave_statistics(std::map<std::string,int> *I_map,std::list <WaveData> *m_count);


/**
 * @brief 域名统计临时函数
 *
 * @param record 原始数据包结构体
 * @param dns_ipaddress 域名统计临时数据结构
 */
void temp_dns_ipaddress_stat(const struct Packet_Record & record,std::list <DnsInfo> *dns_ipaddress);


/**
 * @brief dns_ipaddress_stat 域名统计临时数据结构
 */
	std::list <DnsInfo> m_dns_ipaddress_stat;

/**
 * @brief dns_ipaddress_stat 域名统计临时数据结构
 */
	std::list <DnsInfo> m_filter_dns_ipaddress_stat;

/**
 * @brief m_ST_list_packets 原始数据包临时数据结构
 */
    std::list <Packet_Record> m_ST_list_packets;

	/**
	 * @brief m_P_list_packets 协议统计临时数据结构
	 */
    std::list <ProtocolStatistics> m_P_list_packets;
	/**
	 * @brief m_F_list_packets IP统计临时数据结构
	 */
    std::list <FlowPieData> m_F_list_packets;
	/**
	 * @brief m_temp_IpStatisticsRecord IP统计（IP，包数，大小）数据结构
	 */
    std::list <IpStatisticsRecord> m_temp_IpStatisticsRecord;
	/**
	 * @brief m_host_m_frenquency IP访问频次临时数据结构
	 */
    std::map<std::string,std::list <int> > m_host_m_frenquency;
	/**
	 * @brief m_count IP访问频次临时数据结构
	 */
    std::list <WaveData> m_count;

	/**
	 * @brief m_Filter_P_list_packets 过滤后的协议统计临时数据结构
	 */
    std::list <ProtocolStatistics> m_Filter_P_list_packets;
	/**
	 * @brief m_Filter_F_list_packets 过滤后的IP统计临时数据结构
	 */
    std::list <FlowPieData> m_Filter_F_list_packets;
	/**
	 * @brief m_Filter_temp_IpStatisticsRecord 过滤后IP访问频次临时数据结构
	 */
    std::list <IpStatisticsRecord> m_Filter_temp_IpStatisticsRecord;
	/**
	 * @brief Filter_m_count 过滤后IP访问频次临时数据结构
	 */
    std::list <WaveData> Filter_m_count;
	/**
	 * @brief m_Filter_host_m_frenquency 
	 */
    std::map<std::string,std::list <int> > m_Filter_host_m_frenquency;

	/**
	 * @brief m_Ip_map IP统计临时数据结构
	 */
    std::map<std::string,int> m_Ip_map;
	/**
	 * @brief m_Filter_Ip_map 过滤后的IP统计临时数据结构
	 */
    std::map<std::string,int> m_Filter_Ip_map;

	/**
	 * @brief m_Protocol_map 协议统计临时函数
	 */
    std::map<std::string,int> m_Protocol_map;
	/**
	 * @brief m_Filter_Protocol_map 过滤后的协议统计临时数据结构
	 */
    std::map<std::string,int> m_Filter_Protocol_map;
	/**
	 * @brief m_Counts_map IP统计临时数据结构
	 */
	std::map<std::string,int> m_Counts_map;
	std::map<std::string,int> m_filter_Counts_map;
 
	/**
	 * @brief m_filter 过滤统计的指针变量
	 */
    CIPFilter  * m_filter;
	/**
	 * @brief m_packets_count 统计总包数的变量
	 */
    int m_packets_count;
	/**
	 * @brief m_five_timer_id m_one_timer_id 定时器变量
	 */
    int m_five_timer_id;
	int m_one_timer_id;
	/**
	 * @brief m_Five_counts 五分钟定时次数记录变量
	 */
    int m_Five_counts;
	/**
	 * @brief m_One_counts 每分钟定时次数记录变量
	 */
	int m_One_counts;
	int status_key;
	/**
	 * @brief m_mutex 数据锁 
	 */
	QMutex m_mutex;
	/**
	 * @brief m_mutex_prepare 统计锁
	 */
    QMutex m_mutex_prepare; 
	int packets_count;
	QWaitCondition m_condition;
    QWaitCondition m_wait_finish_condition;

};

#endif
