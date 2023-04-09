#ifndef __COMMON_DEF_H__
#define __COMMON_DEF_H__

#include "base_def.h"

struct ObjId
{
    uint32_t _id;
    uint32_t _thread_index;

    ObjId():_id(0), _thread_index(0){}
};


bool operator < (const ObjId & oj1, const ObjId & oj2);

bool operator==(const ObjId & oj1, const ObjId & oj2);

class normal_msg : public std::enable_shared_from_this<normal_msg>
{
    public:
        normal_msg(){}
        virtual ~normal_msg(){}
        normal_msg(int op):_msg_op(op){}
        int _msg_op;
};

class content_msg: public normal_msg
{
    public:
        content_msg()
        {
            _msg_op = NORMAL_MSG_CONNECT;
            fd = 0;
        }

        virtual ~content_msg(){}
        int fd;
};


struct timer_msg
{
        timer_msg()
        {
            _timer_type = NONE_TIMER_TYPE;
            _time_length = 0;
            _timer_id = 0;
            _obj_id = 0;
        };

        ~timer_msg()
        {
            //PDEBUG("time_length:%u timer_id:%u _timer_type:%u", _time_length, _timer_id, _timer_type);
        }

        /** _obj_id, _timer_type, _time_length 必须传入 , _time_length 为毫秒级别**/
        uint32_t _obj_id;
        uint32_t _timer_type;
        uint32_t _timer_id;
        uint32_t _time_length;
};

struct url_info
{
    std::string url;
    std::string protocol_type;
    std::string domain;
    std::string ip;
    int port;
    std::string full_path;
    std::string path;
    std::string query;

    void reset()
    {
        url.clear();
        protocol_type.clear();
        domain.clear();
        ip.clear();
        port = 80;
        full_path.clear();
        path.clear();
        query.clear();
    }
};

enum http_cmd_type {
    HTTP_REQ_GET     = 1 << 0,
    HTTP_REQ_POST    = 1 << 1,
    HTTP_REQ_HEAD    = 1 << 2,
    HTTP_REQ_PUT     = 1 << 3,
    HTTP_REQ_DELETE  = 1 << 4,
    HTTP_REQ_OPTIONS = 1 << 5,
    HTTP_REQ_TRACE   = 1 << 6,
    HTTP_REQ_CONNECT = 1 << 7,
    HTTP_REQ_PATCH   = 1 << 8
};

enum HTTP_STATUS
{
    RECV_HEAD = 0,
    RECV_BODY = 1,
    SEND_HEAD = 2,
    SEND_BODY = 3
};

struct http_response_code
{
        http_response_code()
        {
            _response_list.insert(std::make_pair(200, "OK"));
            _response_list.insert(std::make_pair(206, "Partial Content"));
            _response_list.insert(std::make_pair(301, "Moved Temporarily"));
            _response_list.insert(std::make_pair(302, "the uri moved temporarily"));
            _response_list.insert(std::make_pair(304, "page was not modified from las"));
            _response_list.insert(std::make_pair(400, "Bad Request"));
            _response_list.insert(std::make_pair(404, "Not Found"));
            _response_list.insert(std::make_pair(403, "Forbidden"));
            _response_list.insert(std::make_pair(409, "Conflict"));
            _response_list.insert(std::make_pair(500, "Internal Server Error"));
            _response_list.insert(std::make_pair(501, "not implemented"));
            _response_list.insert(std::make_pair(503, "the server is not available"));
        }

        ~http_response_code()
        {
        }

        std::string get_response_str(int status_code);

        std::map<int, std::string> _response_list;
        static http_response_code response;
};

struct set_cookie_item
{
    std::string _value;
    std::string _path;
    std::string _domain;
    uint64_t _expire;
    set_cookie_item()
    {
        _expire = 0;
    }
};

struct http_req_head_para
{
    http_req_head_para()
    {
        _method = "GET";
        _version = "HTTP/1.1";
    }

    void init()
    {
        _method.clear();
        _url_path.clear();
        _version.clear();

        _headers.clear();
        _cookie_list.clear();
    }

    std::string * get_header(const char * str)
    {
        std::string * ptr = NULL;
        if (!str) {
            return ptr;
        }

        std::map<std::string, std::string>::iterator it;
        for (it = _headers.begin(); it != _headers.end(); it++) {
            if (strcasestr(it->first.c_str(), str)) {
                ptr = &(it->second);
                break;
            }
        }

        return ptr;
    }

    void to_head_str(std::string * head);


    std::string _method;
    std::string _url_path;
    std::string _version;

    std::map<std::string, std::string> _headers;    
    std::map<std::string, std::string> _cookie_list;
};

struct net_addr
{
    std::string ip;
    int port;

    net_addr()
    {
        port = 0;
    }
};


struct http_res_head_para
{
    http_res_head_para()
    {
        _response_code = 200;
        _response_str = "OK";
        _version = "HTTP/1.1";
    }

    void init()
    {
        _response_code = 200;
        _response_str.clear();
        _version.clear();
        _cookie_list.clear();
        _headers.clear();
        _chunked.clear();
    }

    std::string * get_header(const char * str)
    {
        std::string * ptr = NULL;
        if (!str) {
            return ptr;
        }

        std::map<std::string, std::string>::iterator it;
        for (it = _headers.begin(); it != _headers.end(); it++) {
            if (strcasestr(it->first.c_str(), str)) {
                ptr = &(it->second);
                break;
            }
        }

        return ptr;
    }


    void to_head_str(std::string * head);

    int _response_code;
    std::string _response_str;
    std::string _version;
    std::map<std::string, set_cookie_item> _cookie_list;
    std::map<std::string, std::string> _headers;
    std::string _chunked;
}; 


enum HTTP_RECV_TYPE
{
    CHUNK_TYPE = 0,
    CONTENT_LENGTH_TYPE = 1,
    OTHER_TYPE = 2
};

struct boundary_para
{
    std::string _boundary_str;
    uint32_t _boundary_content_length;
    boundary_para()
    {
        init();
    }

    void init()
    {
        _boundary_str.clear();
        _boundary_content_length = (uint32_t)-1;		
    }
};

const uint32_t BOUNDARY_EXTRA_LEN = 8;


#endif
