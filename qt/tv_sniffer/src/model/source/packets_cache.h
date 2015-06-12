/**@file packets_cache.h
 *@brief 数据包队列
 *@author huxb
 *@version 1.0.0
 *@date 2014-1-24
 */
#ifndef _PACKETS_CACHE_H_
#define _PACKETS_CACHE_H_
#include <list>
#include <QMutex>
#include <QMutexLocker>

struct Packet_Record;
class CPacketsCache
{
	public:
		/**
		 * @brief  向队列添加单个数据包
		 * @param packet_record, 单个数据包
		 */
		void add_packet (const Packet_Record & packet_record);
		/**
		 * @brief  向队列添加多个数据包
		 * @param record_list, 多个数据包
		 */
		void add_packets (const std::list <Packet_Record> & record_list);
		/**
		 * @brief 获取数据包
		 * @param record_list, 将获取到的数据包存放到此list里
		 */
		void get_packets (std::list <Packet_Record> & record_list);
		/**
		 * @brief 清空队列
		 */
		void clear ();

	protected:

	private:
		std::list <Packet_Record> m_list_packets;//包队列
		QMutex m_mutex; 
};

#endif
