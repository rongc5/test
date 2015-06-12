/**@file packets_sender.h
 *@brief 包发送器
 *@author huxb
 *@version 1.0.0
 *@date 2014-1-24
 */
#ifndef _PACKETS_SENDER_H_
#define _PACKETS_SENDER_H_
#include <QObject>
#include <QTimerEvent>
class CPacketsCache;
class CStatisticsThread;
class IModel;


class CPacketsSender : public QObject
{
	public:
		/**
		 * @brief  构造函数
		 * @param cache，数据包队列
		 * @param statistics_thread,统计线程
		 * @param model,
		 */
		CPacketsSender (CPacketsCache * cache,
				CStatisticsThread * statistics_thread,
				IModel * model);
		/**
		 * @brief  开始发送数据包到界面上
		 */
		void start();
		/**
		 * @brief  停止发送数据包到界面上
		 */
		void stop();

	protected:
		/**
		 * @brief 定时器 
		 */
		void timerEvent(QTimerEvent *event);

	private:
		/**
		 * @brief 发送数据到界面上
		 */
		void send ();
	private:
		int               m_timer_id;   //定时器id
		bool              m_stop;       //启动停止标记
		CPacketsCache  * m_packets_cache; //包队列
		CStatisticsThread * m_statistics_thread; //统计线程
		IModel            * m_model;        //mvc中的model
};


#endif
