#ifndef __COMMON_DEF_H__
#define __COMMON_DEF_H__

#include "base_def.h"

enum normal_msg_op
{
    MSG_CONNECT = 1,
    MSG_TIMER,
    MSG_HTTP_REQ,
    MSG_LOG,
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

enum timer_type
{
    NONE_TIMER_TYPE,
    DELAY_CLOSE_TIMER_TYPE,
    WEB_SOCKET_HANDSHAKE_OK_TIMER_TYPE = 10000, 
};


class timer_msg:public normal_msg
{
    public:
        timer_msg()
        {
            _msg_op = MSG_TIMER;
            _timer_type = NONE_TIMER_TYPE;
            _time_length = 0;
            _reach_time = 0;
        };
        virtual ~timer_msg(){}

        ObjId _id;
        timer_type _timer_type;
        uint64_t _time_length;
        uint64_t _reach_time;
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

    log_conf();
};

class log_msg
{
    public:
        log_msg()
        {
            _buf = NULL;
        }
        virtual ~log_msg()
        {
            if (_buf){
                delete _buf;
                _buf = NULL;
            }
        }

        LogType _type;
        vector<char> * _buf;
};

struct url_info
{
    string protocol_type;
    string domain;
    int port;
    string path;
    string query;
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


class http_req_msg:public normal_msg
{
    public:
        http_req_msg()
        {
            _msg_op = MSG_HTTP_REQ;
        };
        virtual ~http_req_msg(){}
          
        http_cmd_type cmd_type;

        string sid;
		  string  url;
		  string post_data;
		  map<string, string> headers;
};

enum HTTP_STATUS
{
    RECV_HEAD = 0,
    RECV_BODY = 1,
    SEND_HEAD = 2,
    SEND_BODY = 3
};

class http_response_code
{
    public:
        http_response_code()
        {
            _response_list.insert(make_pair(200, "OK"));
            _response_list.insert(make_pair(206, "Partial Content"));
            _response_list.insert(make_pair(301, "Moved Temporarily"));
            _response_list.insert(make_pair(302, "the uri moved temporarily"));
            _response_list.insert(make_pair(304, "page was not modified from las"));
            _response_list.insert(make_pair(400, "Bad Request"));
            _response_list.insert(make_pair(404, "Not Found"));
            _response_list.insert(make_pair(403, "Forbidden"));
            _response_list.insert(make_pair(409, "Conflict"));
            _response_list.insert(make_pair(500, "Internal Server Error"));
            _response_list.insert(make_pair(501, "not implemented"));
            _response_list.insert(make_pair(503, "the server is not available"));
        }

        ~http_response_code()
        {
        }
       
		string get_response_str(int status_code)
		{
			map<int, string>::iterator tmp_itr = _response_list.find(status_code);
			if (tmp_itr == _response_list.end())
			{
				THROW_COMMON_EXCEPT("http response code not found");
			}

			return tmp_itr->second;
		}

	private:	
        map<int, string> _response_list;
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
        _host.clear();
        _url_path.clear();
        _version.clear();

        _headers.clear();
        _cookie_list.clear();
    }

    string * get_header(const char * str)
    {
        string * ptr = NULL;
        if (!str) {
            return ptr;
        }

        map<string, string>::iterator it;
        for (it = _headers.begin(); it != _headers.end(); it++) {
            if (strcasestr(it->first.c_str(), str)) {
                ptr = &(it->second);
                break;
            }
        }

        return ptr;
    }

    string _method;
    string _url_path;
    string _version;

    map<string, string> _headers;    
    map<string, string> _cookie_list;
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
        _version.clear();
        _cookie_list.clear();
        _content_length = (uint64_t)-1;
        _headers.clear();
        _chunked.clear();
    }

    string * get_header(const char * str)
    {
        string * ptr = NULL;
        if (!str) {
            return ptr;
        }

        map<string, string>::iterator it;
        for (it = _headers.begin(); it != _headers.end(); it++) {
            if (strcasestr(it->first.c_str(), str)) {
                ptr = &(it->second);
                break;
            }
        }

        return ptr;
    }

    int _response_code;
    string _version;
    map<string, set_cookie_item> _cookie_list;
    uint64_t _content_length;
    map<string, string> _headers;
    string _chunked;
}; 


enum HTTP_RECV_TYPE
{
	CHUNK_TYPE = 0,
	CONTENT_LENGTH_TYPE = 1,
	OTHER_TYPE = 2
};

struct boundary_para
{
	string _boundary_str;
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
