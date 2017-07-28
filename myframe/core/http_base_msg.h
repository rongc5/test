#ifndef __HTTP__BASE__MSG__H__
#define __HTTP__BASE__MSG__H__


#include "base_def.h"

enum HTTP_STATUS
{
    RECV_HEAD = 0,
    RECV_BODY = 1,
    SEND_HEAD = 2,
    SEND_BODY = 3
};

class response_code
{
    public:
        response_code()
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
            _response_list.insert(make_pair(503, "the server is not available"));
        }

        ~response_code()
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

    const string * get_header(const char * str)
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


class http_send_arg
{
	public:
		http_send_arg()
		{
		}
		
		virtual ~http_send_arg()
		{
		}
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
