
#include "ip_filter.h"
#include "data_struct.h"

void CIPFilter::set_filter (const std::list <IpFilterInfo> & ip_filter_info)
{
	m_ip_filter_info.clear();
	m_ip_filter_info = ip_filter_info;
}

bool CIPFilter::filter (const std::string & ip, const std::string & port)
{
	std::list<IpFilterInfo>::iterator it=m_ip_filter_info.begin();

	for(it; it!=m_ip_filter_info.end(); it++)
	{
		//ip and port is not empty	
		if((*it).ip == ip && (*it).port == port)
			return true;

		//ip and port is empty
		if((*it).ip == "" && (*it).port == "")
			continue;

		//port is empty
		if((*it).port == ""){
			if((*it).ip == ip)
				return true;
			else 
				continue;
		}

		//ip is empty
		if((*it).ip == ""){
			if((*it).port == port)
				return true;
			else
				continue;
		}
	}

	return false;
}
