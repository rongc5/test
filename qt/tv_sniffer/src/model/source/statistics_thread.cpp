 /**
 * @file statistics_thread.cpp
 * @brief tvsniffer统计代码，实现部分
 *		  统计分为两部分 1：全量统计部分 2：过滤统计部分
 *		  统计相关子项包含五部分
 *		  1：IP统计
 *		  2：协议统计
 *		  3：IP-次数-大小统计
 *		  4：IP访问频次统计（每五分钟）
 *		  5：IP访问频次统计（每分钟）
 * @author yangl
 * @version 1.0.0
 * @date 2014-02-11
 */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>	
#include "ip_filter.h"
#include "statistics_thread.h"
#include "packets_sender.h"
#include "data_struct.h"
#include "tvsnifferlog.h"

CStatisticsThread::CStatisticsThread (CIPFilter * filter)
    : m_filter (filter)
{
/**************************************
 *  设置定时器,主要用于IP访问频次统计 *
 **************************************/
	m_five_timer_id = startTimer( 10 * 1000); /**	五分钟定时器	**/	
	m_one_timer_id = startTimer(5 * 1000);	 /**	每分钟定时器	**/
}

void CStatisticsThread::timerEvent(QTimerEvent *event)
{
        /* 定时动作 */
    if (event->timerId() == m_five_timer_id) 
	{
		/*  IP访问频次统计 */
		m_Five_counts += 1;	/**	记录统计(每五分钟) 执行次数 **/
//		std::cout << "IP访问频次统计" << std::endl;
        QMutexLocker locker (&m_mutex_prepare);
		Temp_get_frenquency(&m_Ip_map,&m_host_m_frenquency); /**	统计IP访问频次(每五分钟) 临时函数	**/
		Temp_get_frenquency(&m_Filter_Ip_map,&m_Filter_host_m_frenquency); /**	统计过滤后IP访问频次(每五分钟) 临时函数**/
    	}
	if(event->timerId() == m_one_timer_id)
	{
		/* 每分钟统计访问频次 	*/
		m_One_counts += 1; /** 记录统计(每分钟) 执行次数 **/
//		std::cout << "每分钟统计访问频次" << std::endl;
        
        QMutexLocker locker (&m_mutex_prepare);
		Temp_set_wave_statistics(&m_Counts_map,&m_count); /**    统计IP访问频次(每分钟) 临时函数   */
		Temp_set_wave_statistics(&m_filter_Counts_map,&Filter_m_count); /**  统计过滤后IP访问频次(每分钟) 临时函数**/
	}
}
/**
 * @brief 原始数据包接口
 *
 * @param packet_record 原始数据包数据结构
 */
void CStatisticsThread::add_packet (const Packet_Record & packet_record)
{
	QMutexLocker locker (&m_mutex);
	m_ST_list_packets.push_back(packet_record);
	m_packets_count++;
	packets_count++;
	status_key = 1;
	m_condition.wakeOne ();
}
/**
 * @brief 执行统计部分的临时函数
 *
 * @param record 原始数据包临时数据结构 
 */
void CStatisticsThread::process_packets (const struct Packet_Record & record)
{
    
    {
        QMutexLocker locker (&m_mutex_prepare);

        Temp_get_flow_pie_data(record,&m_Ip_map); /**	IP统计	**/
        temp_ip_One(record,&m_Counts_map);	/**	 IP统计，主要用于每分钟IP频次统计	**/
        Temp_get_protocol_statistics(record,&m_Protocol_map); /** 协议统计	**/
        Temp_get_ip_statistics(record,&m_temp_IpStatisticsRecord); /** IP统计(IP,包数，大小) **/
		temp_dns_ipaddress_stat(record,&m_dns_ipaddress_stat); /** 域名统计（目的域名，目的地址）	**/
	}
	

	/********************	
	 *	过滤统计部分	*
	 ********************/
	if(m_filter->filter(record.src_ip,record.src_port))
	{
        QMutexLocker locker (&m_mutex_prepare);
		Temp_get_flow_pie_data(record,&m_Filter_Ip_map); /** 过滤IP统计	**/
        temp_ip_One(record,&m_filter_Counts_map);	/**	 IP统计，主要用于每分钟IP频次统计	**/
		Temp_get_protocol_statistics(record,&m_Filter_Protocol_map);  /** 过滤协议统计	**/
		Temp_get_ip_statistics(record,&m_Filter_temp_IpStatisticsRecord); /** IP统计(IP，包数，大小) **/
		temp_dns_ipaddress_stat(record,&m_filter_dns_ipaddress_stat); /** 域名统计（目的域名，目的地址）	**/
	}
}

/**
 * @brief 全量统计 
 */

/**
 * @brief 对外提供的获取总包数接口
 *
 * @param count 记录总包数变量
 */
void CStatisticsThread::get_packet_count (int &count)
{
	QMutexLocker locker (&m_mutex_prepare);
	count = m_packets_count;
	m_packets_count = 0;
}

void CStatisticsThread::get_report_statistics (ReportStatInfo & report_info)
{
    QMutexLocker locker (&m_mutex);
    {
        while (!m_ST_list_packets.empty())
        {
             m_wait_finish_condition.wait(&m_mutex);
        }
    }
   
    get_flow_pie_data (report_info.flow_pie_data);
    get_protocol_statistics(report_info.protocol_statistics);
    get_ip_statistics (report_info.ip_statistics);
    get_frenquency (report_info.host_frenquency);
    get_wave_statistics (report_info.wave_statistics);
	get_dns_info(report_info.dns_statistics);
    get_filtered_ip_statistics (report_info.filtered_ip_statistics);
    get_filtered_protocol_statistics (report_info.filtered_protocol_statistics);
    get_filtered_flow_pie_data (report_info.filtered_flow_pie_data);  
    get_filtered_frenquency (report_info.filtered_host_frenquency);
    get_filtered_wave_statistics (report_info.filtered_wave_statistics);
	get_filtered_dns_info (report_info.filtered_dns_statistics);
    m_Five_counts = 0;	/**	记录统计(每五分钟) 执行次数 **/
	m_One_counts = 0; /** 定时器（每分钟）计数变量	**/
	status_key = 0;
}


/**
 * @brief 对外提供的获取IP统计数据接口
 *
 * @param host_frenquency IP统计的临时数据结构
 */
#if 0
std::list <FlowPieData> CStatisticsThread::graphics_top_Ten(std::map<std::string,int> *m_map,int level){
	std::list <FlowPieData> Format_PieData;
	std::map<std::string,int>::iterator map_iter; /** m_Ip_map同类型的迭代器 **/
	FlowPieData temp; /** IP统计中的结构体	其中包含1：IP 2：次数 **/
	double temp_data;
	int totalP = 0;
	int Totalnum = 0;
	int Toptennum = 0;
	int totalcount = 0;
	std::list<int> Topten;
	std::list<int>::iterator Topten_iter;
	for(map_iter = (*m_map).begin(); map_iter != (*m_map).end(); map_iter++)
	{
		Topten.push_back((*map_iter).second);
		Totalnum += (*map_iter).second;
	}
	Topten.sort(std::greater<int>());
	for(Topten_iter = Topten.begin(); Topten_iter != Topten.end(); Topten_iter++)
	{
		std::cout << "NEW 饼图：" << *Topten_iter << std::endl;
	}

	for(Topten_iter = Topten.begin(); Topten_iter != Topten.end(); Topten_iter++)
	{
		if(Toptennum < 5)
		{
			for(map_iter = (*m_map).begin(); map_iter != (*m_map).end(); map_iter++)
			{
				if(*Topten_iter == (*map_iter).second)
				{
					temp.ip = (*map_iter).first;
				/********************************************************
				 *	temp_data 是访问次数与总包数商再乘以100进位后的整数 *
				 ********************************************************/
					temp_data = ceil((((double)(*map_iter).second) / Totalnum) * 100);
					temp.data = temp_data;
					Format_PieData.push_back(temp);	
					break;
				}
			}	
			Toptennum++;
			totalcount += *Topten_iter;
		}
		totalP += *Topten_iter;
	}

	temp.ip = "OTHER";
	temp_data = ceil((((double)(Totalnum - totalcount)) / Totalnum) * 100);
	temp.data = temp_data;
	Format_PieData.push_back(temp);	
	std::cout << "清空包数前：" << m_Ip_map.size() << std::endl;
	m_Ip_map.clear();
	std::cout << "清空包数后：" << m_Ip_map.size() << std::endl;
	return Format_PieData;
}
#endif


void CStatisticsThread::get_flow_pie_data (std::list <FlowPieData> & host_frenquency)
{

	std::map<std::string,int>::iterator map_iter; /** m_Ip_map同类型的迭代器 **/
	FlowPieData temp; /** IP统计中的结构体	其中包含1：IP 2：次数 **/
	std::list <FlowPieData> otherdata;
	std::list <FlowPieData>::iterator other_iter;
	double temp_data;
	int totalP = 0;
	int Totalnum = 0;
	int Toptennum = 0;
	int totalcount = 0;
	int othernum = 0;
	std::list<int> Topten;
	std::list<int>::iterator Topten_iter;
	for(map_iter = m_Ip_map.begin(); map_iter != m_Ip_map.end(); map_iter++)
	{
		Topten.push_back((*map_iter).second);
		Totalnum += (*map_iter).second;
	}
	Topten.sort(std::greater<int>());
	for(Topten_iter = Topten.begin(); Topten_iter != Topten.end(); Topten_iter++)
	{
		std::cout << "NEW 饼图：" << *Topten_iter << std::endl;
	}

	for(Topten_iter = Topten.begin(); Topten_iter != Topten.end(); Topten_iter++)
	{
		if(Toptennum < 5)
		{
			for(map_iter = m_Ip_map.begin(); map_iter != m_Ip_map.end(); map_iter++)
			{
				if(*Topten_iter == (*map_iter).second)
				{
					temp.ip = (*map_iter).first;
				/********************************************************
				 *	temp_data 是访问次数与总包数商再乘以100进位后的整数 *
				 ********************************************************/
					temp_data = ceil((((double)(*map_iter).second) / Totalnum) * 100);
					temp.data = temp_data;
					host_frenquency.push_back(temp);	
					break;
				}
			}	
			Toptennum++;
			totalcount += *Topten_iter;
		}
		totalP += *Topten_iter;
	}

/*
	if(m_Ip_map.empty())
	{
		temp.ip = "OTHER";
		temp.data = 100;
		host_frenquency.push_back(temp);	
	}
	else
	{
*/
	for(other_iter = host_frenquency.begin();other_iter != host_frenquency.end();other_iter++)
	{
		othernum += (*other_iter).data;
	}
	temp.ip = "OTHER";
//	temp_data = ceil((((double)(Totalnum - totalcount)) / Totalnum) * 100);
	temp_data = 100 - othernum;
	temp.data = temp_data;
	host_frenquency.push_back(temp);	
//	}
	std::cout << "清空包数前：" << m_Ip_map.size() << std::endl;
	m_Ip_map.clear();
	std::cout << "清空包数后：" << m_Ip_map.size() << std::endl;

}

/**
 * @brief 获取协议统计数据的对外接口 
 *
 * @param protocol_statistics 协议统计临时数据结构
 */
void CStatisticsThread::get_protocol_statistics (std::list <ProtocolStatistics> & protocol_statistics)
{
	std::map<std::string,int>::iterator map_iter;
	ProtocolStatistics temp;
	for(map_iter = m_Protocol_map.begin(); map_iter != m_Protocol_map.end(); map_iter++)
	{
		temp.protocol = (*map_iter).first;
		temp.count = (*map_iter).second;
		protocol_statistics.push_back(temp);	
	}
	m_Protocol_map.clear();
}
/**
 * @brief 获取IP统计数据(IP,包数，大小)对外接口
 *
 * @param ip_statistics IP统计（IP，包数，大小）临时数据结构
 */
void CStatisticsThread::get_ip_statistics (std::list <IpStatisticsRecord> & ip_statistics)
{
	 std::list <IpStatisticsRecord>::iterator list_iter;
/*
	 for(list_iter = m_temp_IpStatisticsRecord.begin(); list_iter != m_temp_IpStatisticsRecord.end(); list_iter++)
	 {
			std::cout << "[get_ip_statistics] IP: " << (*list_iter).ip <<  std::endl;
			std::cout << " [get_ip_statistics] temp.packets: " << (*list_iter).packets << std::endl;
			std::cout << " [get_ip_statistics] temp.bytes: " << (*list_iter).bytes << std::endl; 
	 }
*/
	 std::cout << "包数大小统计(old)：  "<< m_temp_IpStatisticsRecord.size() << std::endl;
	 ip_statistics = m_temp_IpStatisticsRecord;

	 for(list_iter = ip_statistics.begin(); list_iter != ip_statistics.end(); list_iter++)
	 {
			std::cout << "[ip_statistics]  IP: " << (*list_iter).ip << std::endl;
			std::cout << " [ip_statistics]  temp.packets: " << (*list_iter).packets << std::endl;
			std::cout << " [get_ip_statistics]  temp.bytes: " << (*list_iter).bytes << std::endl; 
	 }
	 std::cout << "包数大小统计： (ip_statistics)  "<< ip_statistics.size() << std::endl;
	 m_temp_IpStatisticsRecord.clear();
}
/**
 * @brief 获取单位时间（五分钟）IP访问频次的接口函数
 *
 * @param host_frenquency IP访问频次临时数据结构
 */
void CStatisticsThread::get_frenquency (std::list <HostFrenquency> & host_frenquency)
{
	std::map<std::string,std::list <int> >::iterator host_iter;
	HostFrenquency host_temp;
//	std::cout << "获取单位时间（五分钟）IP访问频次: size() " << m_host_m_frenquency.size() << std::endl;
	for(host_iter = m_host_m_frenquency.begin(); host_iter != m_host_m_frenquency.end(); host_iter++)
	{
		host_temp.host = host_iter->first;
		host_temp.count = host_iter->second;
		host_frenquency.push_back(host_temp);
//		std::cout << "host_frenquency: size() " << host_frenquency.size() << std::endl;
	}
//	std::cout << "[Total] host_frenquency: size() " << host_frenquency.size() << std::endl;
	 m_Five_counts = 0;
	 m_host_m_frenquency.clear();
}
/**
 * @brief 获取访问频次的对外接口
 *
 * @param wave_statistics IP访问频次临时数据结构
 */
void CStatisticsThread::get_wave_statistics (std::list <WaveData> & wave_statistics)
{
	std::cout << "波形图：m_count "<< m_count.size() << std::endl;
	std::list <WaveData>::iterator list_iter;
	WaveData temp;
	std::list<int> Topfive;
	std::list<int>::iterator top_iter;
	std::map<std::string,int> TEMP_map;
	std::map<std::string,int>::iterator temp_iter;
	std::list<std::string> temp_str;
	std::list<std::string>::iterator str_iter;
	std::list <WaveData> format_list;
	int count;
	int five_num = 0;
	for(list_iter = m_count.begin(); list_iter != m_count.end(); list_iter++)
	{
		count = 0;
		std::cout << "波形图: get_wave_statistics  " << (*list_iter).host << std::endl;
		for(top_iter = (*list_iter).count.begin();top_iter != (*list_iter).count.end();top_iter++){
			 count += (*top_iter);
		}
		Topfive.push_back(count);
		TEMP_map.insert(std::pair<std::string,int>((*list_iter).host,count));
	}
	Topfive.sort(std::greater<int>());
	for(top_iter = Topfive.begin();top_iter != Topfive.end();top_iter++){
		if(five_num < 3){
			for(temp_iter = TEMP_map.begin();temp_iter != TEMP_map.end();temp_iter++){
				if(*top_iter == (*temp_iter).second){
					temp_str.push_back((*temp_iter).first); 	
				}
			}
			five_num++;
		}
	}
	for(str_iter = temp_str.begin();str_iter != temp_str.end();str_iter++){
		for(list_iter = m_count.begin(); list_iter != m_count.end(); list_iter++)
		{
			if((*list_iter).host == *str_iter){
				temp.host = (*list_iter).host;
				temp.count = (*list_iter).count;
				wave_statistics.push_back(temp);
				break;
			}
		}
		
	}
	std::cout << "波形图：temp_str "<< temp_str.size() << std::endl;
	std::cout << "波形图：tatal "<< wave_statistics.size() << std::endl;
//	wave_statistics = m_count;
	m_count.clear();
}

/**
 * @brief 过滤统计 
 */

/**
 * @brief 获取过滤后的IP统计（IP，次数，大小）对外接口
 *
 * @param ip_statistics IP统计临时数据结构
 */
void CStatisticsThread::get_filtered_ip_statistics (std::list <IpStatisticsRecord> & ip_statistics)
{
	ip_statistics = m_Filter_temp_IpStatisticsRecord;
	m_Filter_temp_IpStatisticsRecord.clear();
}
/**
 * @brief 获取过滤后的协议统计对外接口 
 *
 * @param protocol_statistics 协议统计临时数据结构
 */
void CStatisticsThread::get_filtered_protocol_statistics (std::list <ProtocolStatistics> & protocol_statistics)
{
	std::map<std::string,int>::iterator map_iter;
	ProtocolStatistics temp;
	for(map_iter = m_Filter_Protocol_map.begin(); map_iter != m_Filter_Protocol_map.end(); map_iter++)
	{
		temp.protocol = (*map_iter).first;
		temp.count = (*map_iter).second;
//		std::cout << "protocol:	" << temp.protocol << std::endl;
//		std::cout << "count:	" << temp.count << std::endl;
		protocol_statistics.push_back(temp);	
	}
	m_Filter_Protocol_map.clear();
}
/**
 * @brief 获取过滤后的IP统计的对外接口
 *
 * @param flow_pie_data IP统计临时数据结构
 */
void CStatisticsThread::get_filtered_flow_pie_data (std::list <FlowPieData> & flow_pie_data)
{
//	std::cout << "获取过滤后的IP访问次数的对外接口" << std::endl;
	std::map<std::string,int>::iterator map_iter;
	FlowPieData temp;
	int Totalpackets = 0;
	if(m_Filter_Ip_map.empty()){
		temp.ip = "OTHER";
		temp.data = 100;
		flow_pie_data.push_back(temp);	
	}
	for(map_iter = m_Filter_Ip_map.begin(); map_iter != m_Filter_Ip_map.end(); map_iter++)
	{
		Totalpackets += (*map_iter).second;
	}
	for(map_iter = m_Filter_Ip_map.begin(); map_iter != m_Filter_Ip_map.end(); map_iter++)
	{
		temp.ip = (*map_iter).first;
		temp.data = ceil((((double)(*map_iter).second)/(Totalpackets))*100);
		flow_pie_data.push_back(temp);	
	}
	m_Filter_Ip_map.clear();
}
/**
 * @brief 获取过滤后的单位时间（五分钟）IP访问频次的接口函数
 *
 * @param host_frenquency IP访问频次临时数据结构
 */
void CStatisticsThread::get_filtered_frenquency (std::list <HostFrenquency> & host_frenquency)
{
	std::map<std::string,std::list <int> >::iterator host_iter;
	HostFrenquency host_temp;
	for(host_iter = m_Filter_host_m_frenquency.begin(); host_iter != m_Filter_host_m_frenquency.end(); host_iter++)
	{
		host_temp.host = host_iter->first;
		host_temp.count = host_iter->second;
		host_frenquency.push_back(host_temp);
	}
	m_Filter_host_m_frenquency.clear();
}
/**
 * @brief 每分钟获取访问频次对外接口（即波形图)
 *
 * @param wave_statistics IP访问频次数据结构
 */
void CStatisticsThread::get_filtered_wave_statistics (std::list <WaveData> & wave_statistics)
{
	std::list <WaveData>::iterator test_iter;
	std::cout << "波形图 size: " << Filter_m_count.size() << std::endl;
	wave_statistics = Filter_m_count;
	for(test_iter = wave_statistics.begin();test_iter != wave_statistics.end(); test_iter++)
	{
		std::cout << "波形图：" << (*test_iter).host << std::endl;
	}
	Filter_m_count.clear();
}
/**
 * @brief 域名统计对外接口
 *
 * @param dns_info 域名统计临时数据结构
 */
void CStatisticsThread::get_dns_info (std::list <DnsInfo> & dns_info)
{
	dns_info = m_dns_ipaddress_stat;
	m_dns_ipaddress_stat.clear();
}
/**
 * @brief 过滤后的域名统计对外接口
 *
 * @param dns_info 域名统计临时数据结构
 */
void CStatisticsThread::get_filtered_dns_info (std::list <DnsInfo> & dns_info)
{
	dns_info = m_filter_dns_ipaddress_stat;
	m_filter_dns_ipaddress_stat.clear();
}

/**
 * @brief private functions 
 */

/**
 * @brief 用于IP访问频次（每分钟）数据的统计
 *
 * @param record 原始数据包数据结构
 * @param Counts_map IP统计的临时数据结构
 */
void CStatisticsThread::temp_ip_One(const struct Packet_Record & record,std::map<std::string,int> *Counts_map)
{
//	std::cout << "统计IP与次数: " << std::endl;
	std::map<std::string,int> m_Ip_map;
	std::map<std::string,int>::iterator map_iter;
	map_iter = (*Counts_map).find(record.src_ip);
	if(map_iter != (*Counts_map).end())
	{
		(*map_iter).second += 1;
	}
	else
	{
		(*Counts_map).insert(std::pair<std::string,int> (record.src_ip,1));
	}
}

/**
 * @brief IP统计(IP，包数) 临时函数
 *
 * @param record 原始数据包结构体
 * @param I_map IP统计临时数据结构
 */
void CStatisticsThread::Temp_get_flow_pie_data(const struct Packet_Record & record,std::map<std::string,int> *I_map)
{
	std::cout <<"NO." << m_Five_counts <<"统计IP与次数: " << (*I_map).size() << std::endl;
	std::map<std::string,int>::iterator map_it;
	map_it = (*I_map).find(record.src_ip);
	if(map_it != (*I_map).end())
	{
		(*map_it).second += 1;
	}
	else
	{
		(*I_map).insert(std::pair<std::string,int> (record.src_ip,1));
	}
//	std::cout << "IP统计：size " << (*I_map).size() << "捕获的总包数： "<< packets_count << std::endl;
/*
	for(map_it = (*I_map).begin(); map_it != (*I_map).end(); map_it++)
	{
		std::cout << "(*I_map) first: " << (*map_it).first << " (*I_map) second: " << (*map_it).second << std::endl;
	}
*/
}

/**
 * @brief 协议统计临时函数
 *
 * @param record 原始数据包结构体
 * @param P_map 协议统计临时数据结构
 */
void CStatisticsThread::Temp_get_protocol_statistics(const struct Packet_Record & record,std::map<std::string,int> * P_map)
{
//	std::cout << "进入协议统计" << std::endl;
	std::map<std::string,int>::iterator map_it;
	ProtocolStatistics temp;
	map_it = (*P_map).find(record.protocol);
	if(map_it != (*P_map).end())
	{
		(*map_it).second += 1;
	}
	else
	{
		(*P_map).insert(std::pair<std::string,int> (record.protocol,1));
	}
}
/**
 * @brief IP统计（IP.次数，大小)数据 临时函数
 *
 * @param record 原始数据包结构体
 * @param m_IpStatisticsRecord IP统计（IP，包数，大小）临时数据结构
 */
void CStatisticsThread::Temp_get_ip_statistics(const struct Packet_Record & record,std::list <IpStatisticsRecord> *m_IpStatisticsRecord)
{
//	std::cout << "IP.次数，大小统计 " << std::endl;
	bool ret = false;
	char str_packets[256] = {0};
	char str_bytes[256] = {0};
	int t_packets = 0;
	int t_bytes = 0;
	std::list <IpStatisticsRecord>::iterator list_iter = (*m_IpStatisticsRecord).begin();
	for(;list_iter != (*m_IpStatisticsRecord).end();list_iter++)
	{
		if((*list_iter).ip == record.src_ip)
		{
			//std::cout << "(*l_it).ip: " << (*list_iter).ip << std::endl;
			ret = true;
			break;
		}
	}
		if(ret == false)
		{
			IpStatisticsRecord temp(record.src_ip, "1", record.packet_size);
			(*m_IpStatisticsRecord).push_back(temp);
		//	std::cout << "record.src_ip: " << record.src_ip << std::endl;
		//	std::cout << "record.packet_size: " << record.packet_size << std::endl;
		}
		else
		{
			t_packets = atoi(((*list_iter).packets).c_str()) + 1;
			sprintf(str_packets,"%d",t_packets);
		//	printf("t_packets -- %d,str_packets --- %s\n",t_packets,str_packets);

			t_bytes = atoi(((*list_iter).bytes).c_str()) + atoi((record.packet_size).c_str());
			sprintf(str_bytes,"%d",t_bytes);
		//	printf("t_bytes -- %d,str_bytes -- %s\n",t_bytes,str_bytes);
			(*list_iter).packets = str_packets;
			(*list_iter).bytes = str_bytes;
		//	std::cout << "(*l_it).packets: " << (*list_iter).packets << std::endl;
		//	std::cout << "(*l_it).bytes: " << (*list_iter).bytes << std::endl;
		}
//	std::cout << "IP.次数，大小统计 : size() " << (*m_IpStatisticsRecord).size() << std::endl;
}


/**
 * @brief 每五分钟统计IP访问频次 临时函数
 *
 * @param I_map IP统计数据存储的临时数据结构
 * @param m_frenquency IP统计频次临时数据结构
 */
void CStatisticsThread::Temp_get_frenquency(std::map<std::string,int> *I_map,std::map<std::string,std::list <int> > *m_frenquency)
{
	std::map<std::string,std::list <int> >::iterator list_iter;
	std::map<std::string,int>::iterator list_packet;
	std::list <int> list_str;
	std::list <int>::iterator list_str1;
	char five_buf[256] = {0};
	memset(five_buf,0,256);
	LOG(DEBUG,five_buf,"[PUSH] COUNTS %d ,MAP size: %d,status_key %d\n",m_Five_counts,(*I_map).size(),status_key);
	if(status_key == 0){
		m_Five_counts = 0;	/**	记录统计(每五分钟) 执行次数 **/
	}

	for(list_packet = (*I_map).begin(); list_packet != (*I_map).end(); list_packet++)
	{
		list_iter = (*m_frenquency).find((*list_packet).first);
		if(list_iter == (*m_frenquency).end())
		{
				list_str.clear();
				if(m_Five_counts == 1)
				{
					list_str.push_back((*list_packet).second);
					(*m_frenquency).insert(std::pair<std::string,std::list<int> >((*list_packet).first,list_str));
				}
				else
				{
					for(int i = 1;i< m_Five_counts;i++)
					{
						list_str.push_back(0);
					}
					list_str.push_back((*list_packet).second);
					(*m_frenquency).insert(std::pair<std::string,std::list<int> >((*list_packet).first,list_str));
				}
		}
		else
		{
			list_iter->second.push_back((*list_packet).second);
		}
	}

	for(list_iter = (*m_frenquency).begin();list_iter != (*m_frenquency).end();list_iter++)
	{
		//list_str.clear();
		list_packet = (*I_map).find(list_iter->first);	
		if(list_packet == (*I_map).end())
		{
			list_iter->second.push_back(0);
			LOG(DEBUG,five_buf,"[PUSH] the IP is %s \n",list_iter->first.c_str());
		}
//		list_str = list_iter->second;
#if 0
		for(list_str1 = list_str.begin();list_str1 != list_str.end();list_str1++)
		{
			LOG(DEBUG,five_buf,"the IP is %s ,the count is %d \n",list_iter->first.c_str(),*list_str1);

		}
#endif
	}
}
/**
 * @brief 每分钟访问频次统计 临时函数
 *
 * @param Counts_map IP统计数据存储的临时数据结构
 * @param m_count IP访问频次的临时数据结构
 */
void CStatisticsThread::Temp_set_wave_statistics(std::map<std::string,int> *Counts_map,std::list <WaveData> *m_count)
{

//	std::cout << "每分钟访问频次统计: size() "<< (*m_count).size()  << std::endl;
//	std::cout << "NO." << m_One_counts << std::endl;
	std::map<std::string,int>::iterator list_iter;
	std::list<int> list_temp;
	std::list<int>::iterator temp_iter;
	std::list <WaveData>::iterator list_str;
	WaveData temp;
	if(status_key == 0){
		m_One_counts = 0; /** 记录统计(每分钟) 执行次数 **/
	}
	bool ret = false;
	for(list_iter = (*Counts_map).begin(); list_iter != (*Counts_map).end(); list_iter++)
	{
		for(list_str = (*m_count).begin(); list_str != (*m_count).end(); list_str++)
		{
			if((*list_str).host == (*list_iter).first)
			{
				ret = true;
				(*list_str).count.push_back((*list_iter).second);
				break;
			}
		}
		if(ret == false)
		{
			list_temp.clear();
			if(m_One_counts == 1)
			{
				temp.host = (*list_iter).first;
				temp.count.push_back((*list_iter).second);
				(*m_count).push_back(temp);
			break;
			}
			else
			{
				temp.host = (*list_iter).first;
				for(int i = 0;i < m_One_counts;i++)
				{
					list_temp.push_back(0);
				}
				temp.count = list_temp;
				(*m_count).push_back(temp);
			}
		}
	}
/*	反向检查，没有的补0*/
		for(list_str = (*m_count).begin(); list_str != (*m_count).end(); list_str++)
		{
			list_iter = (*Counts_map).find((*list_str).host);
			if(list_iter == (*Counts_map).end())
			{
				(*list_str).count.push_back(0);
			}
		}
	(*Counts_map).clear();
} 

void CStatisticsThread::temp_dns_ipaddress_stat(const struct Packet_Record & record,std::list <DnsInfo> *dns_ipaddress)
{
	std::list <DnsInfo>::iterator dns_iter;
	std::map<std::string,std::string> Temp_map;
	std::map<std::string,std::string>::iterator map_iter;
	bool stat = false;
	DnsInfo temp_info;
	if(record.dst_dns.empty())
	{
		std::cout << "域名为空" << std::endl;
	}
	else
	{
		std::cout << "域名 不为空" << std::endl;
#if 0
		map_iter = Temp_map.find(record.dst_ip);
		if(map_iter != Temp_map.end())
		{
			std::cout << "IP: " << map_iter->first << " dns: " << map_iter->second << std::endl;	
		}else{
			Temp_map.insert(std::pair<std::string,std::string>(record.dst_ip,record.dst_dns));
		}
		std::cout << "map size: " << Temp_map.size() << std::endl;
#endif
		if((*dns_ipaddress).empty())
		{
			temp_info.domain_name = record.dst_dns;
			temp_info.ip = record.dst_ip;
			(*dns_ipaddress).push_back(temp_info);
			/* map	*/
//			Temp_map.insert(std::pair<std::string,std::string>(record.dst_dns,record.dst_ip));
		}

		for(dns_iter = (*dns_ipaddress).begin(); dns_iter != (*dns_ipaddress).end(); dns_iter++)
		{
			std::cout << "MAPDNS:  " << (*dns_iter).domain_name << " old dns: " << record.dst_dns << std::endl;
			if((*dns_iter).domain_name == record.dst_dns)
			{
//				(*dns_iter).ip += record.dst_ip;
				stat = true;
				break;
			}

		}
		std::cout << "STATES: " << stat << std::endl;
		if(stat == false)
		{
			std::cout << "----------- stat: " << stat << std::endl;
			temp_info.domain_name = record.dst_dns;
			temp_info.ip = record.dst_ip;
			(*dns_ipaddress).push_back(temp_info);
		}

	}
}

void CStatisticsThread::run ()
{
	m_Five_counts = 0;	/**	定时器（五分钟）计数变量  **/
	m_One_counts = 0; /** 定时器（每分钟）计数变量	**/
	packets_count = 0;
	while(1)
	{
		Packet_Record  TEMP_packet_record; /** 原始数据包结构体	**/
		std::cout << "===========[ begin ]============" << std::endl;
		{
			QMutexLocker locker (&m_mutex);
			if(m_ST_list_packets.empty())
			{
				m_condition.wait(&m_mutex);
			}
			TEMP_packet_record = m_ST_list_packets.front(); /**	返回list m_ST_list_packets 中第一个元素**/
			m_ST_list_packets.pop_front(); /** 删除list m_ST_list_packets中第一个元素	**/
		}
		std::cout << "process_packets ----- begin() " << std::endl;
		string str1("/home/haier/tv_sniffer/src/model/source/log4cxx.properties");
#if 0
		char buf[512] = {0};
		memset(buf,0,512);
		LOG(DEBUG,buf,"[LOG] the num is %d,the string is %s",1267,"UIOS");
#endif
		process_packets(TEMP_packet_record); /**  进行统计	**/	
		std::cout << "===========[ end ]================" << std::endl;
        m_wait_finish_condition.wakeOne();

	}
}
