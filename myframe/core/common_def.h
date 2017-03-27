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


time_t get_date_str(char dest[], size_t dest_len, const char * format);

uint64_t GetTickCount();


enum LogType {
    LOGOFF,
    LOGFATAL,
    LOGWARNING,
    LOGNOTICE,
    LOGTRACE,
    LOGDEBUG,
    LOGSIZE
};


struct log_msg {
    LogType type; 
    time_t st;
    string str;
};

void get_proc_name(char buf[], size_t buf_len);

struct log_conf{
    uint32_t file_max_size;
    char log_path[SIZE_LEN_256];
    char prefix_file_name[SIZE_LEN_256];
    LogType type;
    int bucketlen;
    int deal_mode; // 1 write log 2 print log 3 write && print

    log_conf()
    {
        file_max_size = DEFAULT_LOG_MAX_SIZE;
        strcpy(log_path, "logs");
        get_proc_name(prefix_file_name, sizeof(prefix_file_name));
        type = LOGDEBUG;
        bucketlen = DEFAULT_LOG_BUCKETLEN;
        deal_mode = 1;
    }
};

template<class OBJ>
class REC_OBJ
{
    public:
        REC_OBJ(OBJ * p_obj):_p_obj(p_obj){}
        ~REC_OBJ(){
            if (_p_obj){
                delete _p_obj;
            }
        }
    private:
        OBJ *_p_obj;
};


void set_unblock(int fd);



/**************** ***********************/

enum pass_msg_op
{
    PASS_NEW_FD,
    PASS_NEW_CONNECT = 1,
    PASS_NEW_MSG
};

struct ObjId
{
    uint32_t _id;
    uint32_t _thread_index;

    ObjId():_id(0), _thread_index(0){}
};

bool operator < (const ObjId & oj1, const ObjId & oj2);

bool operator==(const ObjId & oj1, const ObjId & oj2);

struct normal_msg 
{
    normal_msg(){}
    virtual ~normal_msg(){}
};

struct pass_msg //内部传递的消息
{
    ObjId _src_id;
    ObjId _dest_id;
    pass_msg_op _p_op;
    uint32_t _flag; // 是否需要删除p_msg, 0 不需要, 1需要
    normal_msg * p_msg;


    pass_msg():_flag(1), p_msg(NULL){}

    virtual ~pass_msg(){
        if (_flag && p_msg) {
            delete p_msg;
            p_msg = NULL;
        }
    }
};

struct recv_msg: public normal_msg
{
    uint32_t _len;
    char * _ptr;
    uint32_t _flag; // 是否需要删除, 1 需要, 0 不需要

    recv_msg():_len(0), _ptr(NULL), _flag(1){
    }

    virtual ~recv_msg() {
        if (_flag && _ptr) {
            delete [] _ptr;
            _ptr = NULL;
        }
    }
};

struct recv_msg_fd: public normal_msg
{
    int fd;
    recv_msg_fd():fd(0){}

    virtual ~recv_msg_fd(){}
};

#endif
