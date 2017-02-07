#ifndef __COMMON_DEF_H__
#define __COMMON_DEF_H__

#include "base_def.h"


class to_string {
    public:
        to_string(){}
        virtual ~to_string(){}
        virtual const char * to_str(char * dst, size_t dst_len)=0;				
};


struct host_str {
    string  _ip;
    uint16_t _port;
};

enum CONNECT_STATUS
{
    CLOSED = 0,
    CONNECT_OK = 1,
    CONNECTING = 2  
};

enum EPOLL_TYPE
{
    EPOLL_LT_TYPE = 0,
    EPOLL_ET_TYPE 
};


enum RECV_MSG_STATUS
{
    RECV_MSG_HEAD = 0,
    RECV_MSG_BODY 
};


int get_date_str(char dest[], size_t dest_len, const char * format);

uint64_t GetTickCount();


bool operator < (const ObjId & oj1, const ObjId & oj2);




#endif
