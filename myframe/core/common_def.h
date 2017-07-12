#ifndef __COMMON_DEF_H__
#define __COMMON_DEF_H__

#include "base_def.h"
#include "common_util.h"

enum RECV_MSG_STATUS
{
    RECV_MSG_HEAD = 0,
    RECV_MSG_BODY 
};

enum LogType {
    LOGFATAL,
    LOGWARNING,
    LOGNOTICE,
    LOGTRACE,
    LOGDEBUG,
    LOGSIZE
};


struct log_conf{
    uint32_t file_max_size;
    char log_path[SIZE_LEN_256];
    char prefix_file_name[SIZE_LEN_256];
    LogType type;
    int deal_mode; // 1 write log 2 print log 3 write && print

    log_conf()
    {
        file_max_size = DEFAULT_LOG_MAX_SIZE;
        strcpy(log_path, "logs");
        get_proc_name(prefix_file_name, sizeof(prefix_file_name));
        type = LOGDEBUG;
        deal_mode = 1;
    }
};

/**************** ***********************/

enum normal_msg_op
{
    MSG_CONNECT = 1,
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

class normal_msg 
{
    public:
        normal_msg(){}
        virtual ~normal_msg(){}
        normal_msg_op _msg_op;
};


class normal_obj_msg //内部传递的消息
{
    public:
        ObjId _id;
        normal_msg * p_msg;

        virtual ~normal_obj_msg(){
        }
};

struct http_req_head_para
{
    http_req_head_para()
    {
        _content_length = (uint64_t)-1;
        _method = "GET";
    }

    void init()
    {
        _method.clear();
        _url_para_list.clear();
        _cookie_list.clear();
        _content_length = (uint64_t)-1;
        _url_path.clear();
        _host.clear();
        _headers.clear();

        _content_type.clear();
        _connect_type.clear();
    }
    string _method;
    string _url_path;
    string _host;
    map<string, string> _url_para_list;
    map<string, string> _cookie_list;
    uint64_t _content_length;
    map<string, string> _headers;    
    string _content_type;
    string _connect_type;
};

struct set_cookie_item
{
    string _value;
    string _path;
    string _domain;
    uint64_t _expire;
    set_cookie_item()
    {
        _expire = 0;
    }
};

struct http_res_head_para
{
    http_res_head_para()
    {
        _response_code = 200;
        _content_length = (uint64_t)-1;
    }

    void init()
    {
        _response_code = 200;
        _cookie_list.clear();
        _content_length = (uint64_t)-1;
        _headers.clear();
        _chunked.clear();
    }

    int _response_code;
    map<string, set_cookie_item> _cookie_list;
    uint64_t _content_length;
    map<string, string> _headers;
    string _chunked;
}; 





#endif
