
#include "packets_cache.h"
#include "data_struct.h"

void CPacketsCache::add_packet (const Packet_Record & packet_record)
{
	QMutexLocker locker(&m_mutex);
	m_list_packets.push_back(packet_record);
	
}
void CPacketsCache::add_packets (const std::list <Packet_Record> & recore_list)
{
	QMutexLocker locker(&m_mutex);

	std::list<Packet_Record>::const_iterator it = recore_list.begin();
	for(it; it!=recore_list.end(); it++)
	{
		m_list_packets.push_back(*it);
	}


}
void CPacketsCache::get_packets(std::list <Packet_Record> & recore_list)
{
	QMutexLocker locker(&m_mutex);
	recore_list = m_list_packets;

}
void CPacketsCache::clear()
{
	QMutexLocker locker(&m_mutex);
	m_list_packets.clear();

}
