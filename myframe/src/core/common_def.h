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



/***********  zeromq ****************************/

ass zmq_base {

    public:
        virtual int init(char * path);
        virtual int do_send(char * buf, size_t len);
        virtual int do_recv(char * buf, size_t len);
        virtual ~zmq_base(){ 
            zmq_close (zmq_fd);
            zmq_ctx_destroy(context);
        }   

    protected:
        char zeromq_address[PATH_MAX_LEN];
        void * context;
        void *zmq_fd;

};

class zmq_client : public zmq_base{
    public:
        virtual int init(char * path);
};


class zmq_server : public zmq_base{
    public:
        virtual int init(char * path);
};


#endif
