
#ifndef _MODEL_PRINT_H_
#define _MODEL_PRINT_H_
#include "data_struct.h"

inline std::ostream & operator<< (std::ostream & o, const FlowPieData & data)
{
    o << "\n";
    o << "ip = " <<data.ip.c_str () << " data = " << data.data;
    o << "\n";
    return o;
}

inline std::ostream & operator<< (std::ostream & o, const HostFrenquency & data)
{
    o << "\n";
    o << "host = " << data.host.c_str () << "\n";
    std::list <int>::const_iterator cit = 
        data.count.begin ();
    for ( ; cit != data.count.end (); ++cit)
    {
        o << " " << *cit;
    }
    o << "\n";
     return o;
}

inline std::ostream & operator<< (std::ostream & o, const IpStatisticsRecord & data)
{
    o << "\n";
    o << "ip = " <<data.ip.c_str () << " packets = " << data.packets.c_str () << " bytes = " << data.bytes.c_str();
    o << "\n";
     return o;
}

inline std::ostream & operator<< (std::ostream & o, const ProtocolStatistics & data)
{
    o << "\n";
    o << "protocol = " <<data.protocol.c_str () << " count = " << data.count;   
    o << "\n";
     return o;
}

inline std::ostream & operator<< (std::ostream & o, const WaveData & data)
{
    o << "\n";
    o << "host = " <<  data.host.c_str () << "\n";
    std::list <int>::const_iterator cit = 
        data.count.begin ();
    for ( ; cit != data.count.end (); ++cit)
    {
        o << " " << *cit;
    }
    o << "\n";
     return o;
}

inline std::ostream & operator<< (std::ostream & o, const DnsInfo & data)
{
    o << "\n";
    o << "domain_name = " <<data.domain_name.c_str () << " ip = " << data.ip.c_str ();   
    o << "\n";
    return o;
}
 
template<class T>
struct PrintData
{
    inline void operator()(const T & data)
    {
        std::cout << data;
    }
};

inline std::ostream & operator<< (std::ostream & o, const ReportStatInfo & data)
{
    std::cout << "filtered flow_pie_data \n";
    std::for_each(data.filtered_flow_pie_data.begin(), data.filtered_flow_pie_data.end (), PrintData<FlowPieData>());

    std::cout << "filtered host_frenquency \n";
    std::for_each(data.filtered_host_frenquency.begin(), data.filtered_host_frenquency.end (), PrintData<HostFrenquency>());

    std::cout << "filtered ip_statistics \n";
    std::for_each(data.filtered_ip_statistics.begin(), data.filtered_ip_statistics.end (), PrintData<IpStatisticsRecord>());

    std::cout << "filtered protocol_statistics \n";
    std::for_each(data.filtered_protocol_statistics.begin(), data.filtered_protocol_statistics.end (), PrintData<ProtocolStatistics>());

    std::cout << "filtered wave_statistics \n";
    std::for_each(data.filtered_wave_statistics.begin(), data.filtered_wave_statistics.end (), PrintData<WaveData>());

    std::cout << "filtered dns info \n";
    std::for_each(data.filtered_dns_statistics.begin(), data.filtered_dns_statistics.end (), PrintData<DnsInfo>());


    std::cout << " flow_pie_data \n";
     std::for_each(data.flow_pie_data.begin(), data.flow_pie_data.end (), PrintData<FlowPieData>());

     std::cout << " host_frenquency \n";
     std::for_each(data.host_frenquency.begin(), data.host_frenquency.end (), PrintData<HostFrenquency>());

     std::cout << " ip_statistics \n";
     std::for_each(data.ip_statistics.begin(), data.ip_statistics.end (), PrintData<IpStatisticsRecord>());

     std::cout << "protocol_statistics \n";
     std::for_each(data.protocol_statistics.begin(), data.protocol_statistics.end (), PrintData<ProtocolStatistics>());

     std::cout << "wave_statistics \n";
     std::for_each(data.wave_statistics.begin(), data.wave_statistics.end (), PrintData<WaveData>());

     std::cout << "dns info \n";
     std::for_each(data.dns_statistics.begin(), data.dns_statistics.end (), PrintData<DnsInfo>());

    

    return o;
}

#endif