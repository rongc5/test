
#include <iostream>
#include "packets_sender.h"
#include "packets_cache.h"
#include "imodel.h"
#include "data_struct.h"
#include "statistics_thread.h"

CPacketsSender::CPacketsSender (
		CPacketsCache * cache,
		CStatisticsThread * statistics_thread,
		IModel * model)
{
	m_packets_cache 	= cache;
	m_statistics_thread 	= statistics_thread;
	m_model 		= model;
	m_stop			= true;

	m_timer_id = startTimer(1000); 

}

void CPacketsSender::start()
{
	m_stop = false;
}

void CPacketsSender::stop()
{
	m_stop = true;
	send();
}


void CPacketsSender::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == m_timer_id) {
			if(!m_stop)
				send();
	}
}

void CPacketsSender::send ()
{
	std::list <Packet_Record>  packtlist;

	m_packets_cache->get_packets(packtlist);
	m_packets_cache->clear();

	int packet_count=0;
	m_statistics_thread->get_packet_count(packet_count);	
	//m_statistics_thread->packets_count_clear();
    std::cout << "packet_count = " << packet_count << "\n";
	m_model->add_packet_count(packet_count);

	std::list<Packet_Record>::iterator it = packtlist.begin();
	for(it; it!=packtlist.end(); it++)
	{
		m_model->add_packet(*it);
	}
}
