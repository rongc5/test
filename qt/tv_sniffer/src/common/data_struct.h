
#ifndef _DATA_STRUCT_H_
#define _DATA_STRUCT_H_
#include <string>
#include <QDateTime>
struct  Packet_Record
{
    std::string no;
    std::string time;
    std::string src_ip;
    std::string dst_ip;
    std::string src_dns;
    std::string dst_dns;
    std::string src_port;
    std::string dst_port;
    std::string protocol;
    std::string packet_size;

};
struct IpStatisticsRecord
{
    IpStatisticsRecord (
        const std::string & _ip, 
        const std::string & _packets,
        const std::string & _bytes
        ) : ip (_ip),
        packets (_packets),
        bytes (_bytes)
    {

    }

    std::string ip;
    std::string packets;
    std::string bytes;
};
struct  FilterRecord
{
    FilterRecord ()
    {

    }
    FilterRecord (
        const std::string & _name,
        const std::string & _ip,
        const std::string & _port)
        : name (_name),
        ip (_ip),
        port (_port)
    {}
    std::string name;
    std::string ip;
    std::string port;
};

struct ProtocolStatistics
{
    std::string protocol;
    int count;
};

struct HostFrenquency
{
    std::string host;
    std::list <int> count;
};

struct FlowPieData
{
    std::string ip;
    int data;
};

struct BasicInfo 
{
    std::string time;
    std::string sniffer_id;
    std::string user_id;
    std::string mac;
    std::string pf;
    std::string tv_ip;
    std::string tv_Mac;
    std::string local_Ip;
    std::string net_name;
    std::string report_id;
};

struct IpFilterInfo
{
    std::string name;
    std::string ip;
    std::string port;
};

struct PieElement
{
    std::string name;
    int data;
};

struct WaveData 
{
    std::string host;
    std::list <int> count;
};

struct DnsInfo 
{
    std::string domain_name;
    std::string ip;
};

struct ReportStatInfo 
{
    std::list <IpStatisticsRecord> ip_statistics;
    std::list <IpStatisticsRecord>  filtered_ip_statistics;
    std::list <ProtocolStatistics> protocol_statistics;
    std::list <ProtocolStatistics> filtered_protocol_statistics;
    std::list <HostFrenquency>  host_frenquency;
    QDateTime start;
    QDateTime end;
    std::list <HostFrenquency>  filtered_host_frenquency;
    std::list <FlowPieData>  flow_pie_data;
    std::list <FlowPieData> filtered_flow_pie_data;
    std::list <WaveData>  wave_statistics;
    std::list <WaveData>  filtered_wave_statistics;
    std::list <DnsInfo> dns_statistics;
    std::list <DnsInfo> filtered_dns_statistics;
};



#endif
