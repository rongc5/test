
#ifndef _COMMUNICATE_DATA_STRUCT_H_
#define _COMMUNICATE_DATA_STRUCT_H_

#define MESSAGE_START_CAPTURE 1
#define MESSAGE_STOP_CAPTURE  2

struct Message
{
    int type;
    char * content;
    int length;
};


typedef struct _tv_sniffer_column
{
    char no[10];
    char time[10];
    char src_ip[48];
    char dst_ip[48];
    char src_dns[128];
    char dst_dns[128];
    char src_port[6];
    char dst_port[6];
    char protocol[10];
    char packet_size[6];
}tv_sniffer_column;


#endif