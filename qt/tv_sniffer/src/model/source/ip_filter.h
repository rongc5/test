/** 
 * Copyright (C) 2007-2013 Haier
 *
 * @file             ip_filter.h
 * @brief            ip¹ýÂË
 * @author           zhengyl
 * @version	         1.0
 * @date	         2014-1-24
 */
#ifndef _IP_FILTER_H_
#define _IP_FILTER_H_
#include <list>
#include <string>
struct IpFilterInfo;

class CIPFilter
{
public:
     void set_filter (const std::list <IpFilterInfo> & ip_filter_info);
     bool filter (const std::string & ip, const std::string & port);
private:

	std::list <IpFilterInfo> m_ip_filter_info;
};


#endif
