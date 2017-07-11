#ifndef __HTTP_BASE_PROCESS_H__
#define __HTTP_BASE_PROCESS_H__

#include "base_def.h"
#include "base_connect.h"
#include "base_singleton.h"
#include "base_data_process.h"
#include "common_exception.h"

enum HTTP_STATUS
{
    RECV_HEAD = 0,
    RECV_BODY = 1,
    SEND_HEAD = 2,
    SEND_BODY = 3
};


struct http_head_para
{
    http_head_para()
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
        _other_list.clear();
		
		_content_type.clear();
		_connect_type.clear();
    }
    string _method;
    string _url_path;
    string _host;
    map<string, string> _url_para_list;
    map<string, string> _cookie_list;
    uint64_t _content_length;
    map<string, string> _other_list;	
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
        _other_res_list.clear();
		_chunked.clear();
    }

    int _response_code;
    map<string, set_cookie_item> _cookie_list;
    uint64_t _content_length;
    map<string, string> _other_res_list;
	string _chunked;
};

class http_connect_info:public connect_info
{
	public:
		http_head_para _head_para;
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

template<class DATA_PROCESS>
class http_base_process: public base_data_process
{
    public:
        http_base_process(base_connect *p):base_data_process(p)
        {
            _send_head_len = 0;
            //_data_process = NULL;
			_send_status = 0;			
        }

        virtual ~http_base_process()
        {
            if (_data_process != NULL)
                delete _data_process;
        }

        virtual size_t process_recv_buf(char *buf, const size_t len)
        {	
            if (_http_status > RECV_BODY)
            {
                THROW_COMMON_EXCEPT("http recv status not correct " << _http_status);
            }
			

            size_t ret = 0;
            bool staus_change = false;
            string left_str;
            if (_http_status == RECV_HEAD)
            {
                _recv_head.append(buf, len);
                check_head_finish(left_str);
                staus_change = true;				
            }

            if (_http_status == RECV_BODY) //避免数据不被拷贝多次
            {	
                int result = 0;
                if (staus_change)
                {
                    ret = len - left_str.length();
                    ret += process_recv_body((char*)left_str.c_str(), left_str.length(), result);
                }
                else	
                    ret = process_recv_body(buf, len, result);

                if (result == 1)
                {
                    recv_finish();
                }
            }			
            return ret;
        }

        void handle_timeout(const uint32_t timer_type)
        {
        }

		void set_para()
		{
		}

        string* get_send_buf()
        {
            if (_http_status < SEND_HEAD)
            {
                WRITE_WARN("http send status not correct (%d)", _http_status);
				_send_status = 0;
				return NULL;
            }
			
			string *ret_str = NULL;
            if (_http_status == SEND_HEAD)
            {
                if (_send_head.empty())
                {
                    gen_send_head();
                }

				if (_send_head.empty())
					return NULL;

				ret_str = new string();
                ret_str->append(_send_head);				
                _http_status = SEND_BODY;
				return ret_str;
            }
			else if (_http_status == SEND_BODY)
            {
                int result = 0;
                ret_str = _data_process->get_send_body(result);
                if (result == 1)
                    send_finish();
            }
			
			
			if (ret_str == NULL)
				_send_status = 0;
			
            return ret_str;
        }

        void process_s(normal_obj_msg *p_msg)
        {
            _data_process->process_s(p_msg);
        }	
        
        virtual void reset()
        {						
            _send_head_len = 0;
            if (_data_process != NULL)
            {
                delete _data_process;
                _data_process = NULL;
            }
            _recv_head.clear();
        }

		void routine()
		{			
		}
		
		
		/****************************以上是五个口子，以下是供底层调用********************************************/
        string &get_head()
        {
            return _recv_head;
        }

        NET_OBJ *get_connect()
        {
            return _p_connect;
        }

		void change_http_status(HTTP_STATUS status, bool if_change_send = true)
		{
			_http_status = status;
			if (status == SEND_HEAD && if_change_send)
			{
				change_to_cansend();
			}
		}

		
		void change_to_cansend()
		{
			if (_send_status == 0)
			{
				_send_status = 1;

				_p_connect->add_event(EPOLLOUT);
			}
		}

		virtual void peer_close() = 0;

		DATA_PROCESS *get_process()
		{
			return _data_process;
		}
		static void parse_url_para(const string &url_para, map<string, string> &url_para_map)
		{
            vector<string> vec_str;
            CToolKit::SplitString(url_para, "&", vec_str);
            size_t num = vec_str.size();
            for (size_t ii = 0; ii < num; ii ++)
            {
                vector<string> tmp_vec;
                CToolKit::SplitString(vec_str[ii], "=", tmp_vec);
                if (tmp_vec.size() == 2)
                {
                    CToolKit::StringTrim(tmp_vec[0]);
                    CToolKit::StringTrim(tmp_vec[1]);
					string tmp_para;
					CToolKit::UrlDecode(tmp_vec[1], tmp_para);
                    url_para_map.insert(make_pair(tmp_vec[0], tmp_para));
                }
            }				
		}

              void on_connect_comming(){}
    protected:		
		virtual size_t process_recv_body(char *buf, size_t len, int &result) = 0;	
		
        virtual void parse_header() = 0;
        virtual void gen_send_head() = 0;
        virtual void recv_finish() = 0;
        virtual void send_finish() = 0;

        void check_head_finish(string &left_str)
        {
            size_t pos =  _recv_head.find("\r\n\r\n");
            if (pos != string::npos)
            {
                left_str = _recv_head.substr(pos + 4);
                _recv_head.erase(pos + 4);				
                //_http_status = RECV_BODY;
				change_http_status(RECV_BODY);
                parse_header();
				
				_data_process->header_recv_finish();
            }
            else
            {
                if (_recv_head.length() > MAX_HTTP_HEAD_LEN) //http头不要超过10k
                {
                    THROW_COMMON_EXCEPT("http head too long (" << _recv_head.length() << ")")
                }
            }			
        }

        HTTP_STATUS _http_status;
        string _recv_head;
        string _send_head;
        size_t _send_head_len;
        DATA_PROCESS *_data_process;
		int _send_status;	
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
template<class DATA_PROCESS>
class http_res_process:public http_base_process<DATA_PROCESS>
{
    public:
        http_res_process(NET_OBJ *p):http_base_process<DATA_PROCESS>(p)
	    {
	        http_base_process<DATA_PROCESS>::change_http_status(RECV_HEAD);
			_recv_body_length = 0;
			_recv_boundary_status = BOUNDARY_RECV_HEAD;
			http_base_process<DATA_PROCESS>::_data_process = DATA_PROCESS::gen_process(this);
	    }

        ~http_res_process()
        {		
        }

        http_head_para &get_req_head_para()
        {
            return _req_head_para;
        }

        http_res_head_para &get_res_head_para()
        {
            return _res_head_para;
        }

		void set_res_head_para(const http_res_head_para &para)
		{
			_res_head_para = para;
		}

		void reset()
        {
            http_base_process<DATA_PROCESS>::reset();        
            http_base_process<DATA_PROCESS>::change_http_status(RECV_HEAD);
            _req_head_para.init();
            _res_head_para.init();
			
			_boundary_para.init();
			_recv_body_length = 0;
			_recv_boundary_status = BOUNDARY_RECV_HEAD;

					
			http_base_process<DATA_PROCESS>::_data_process = DATA_PROCESS::gen_process(this);
        }

		void peer_close() //被动端对端关了， 啥也不用干
		{			
		}

		
		string gen_res_head()
		{
			stringstream ss;
            //返回状态码
           	string response_str = response_code::_s_response_code.get_response_str(_res_head_para._response_code);
			
            ss << "HTTP/1.1 " << _res_head_para._response_code << " " << response_str << "\r\n";          			
            //返回服务器名称
            ss << "Server: KWS1.0\r\n";
			ss << "Connection: close\r\n";  //暂时不支持keep_alive

            //cookie
            if (_res_head_para._cookie_list.size() > 0)
            {
                for (map<string, set_cookie_item>::iterator itr = _res_head_para._cookie_list.begin(); 
                        itr != _res_head_para._cookie_list.end(); ++itr)
                {
	                ss << "Set-Cookie: ";
					ss << itr->first << "=" << itr->second._value;
					if (itr->second._expire != 0)
					{
						ss << ";expires=" << CToolKit::SecToHttpTime(itr->second._expire);
					}
					
					if (itr->second._path != "")
					{
						ss << ";path=" << itr->second._path;
					}
					
					if (itr->second._domain != "")
					{
						ss << ";domain=" << itr->second._domain;
					}
					ss << "\r\n";
                }
            }

            //content_length
            if (_res_head_para._content_length != (uint64_t)-1)
            {
                ss << "Content-Length: " << _res_head_para._content_length << "\r\n";
            }

            //other para
            for (map<string, string>::iterator itr = _res_head_para._other_res_list.begin(); 
                    itr != _res_head_para._other_res_list.end(); ++itr)
            {
                ss << itr->first << ": " << itr->second << "\r\n";
            }
            //最后一个分隔符
            ss << "\r\n";
			return ss.str();
		}
    protected:
		virtual size_t process_recv_body(char *buf, size_t len, int &result)
		{
			int ret = 0;
			if (strcasecmp(_req_head_para._method.c_str(), "GET") == 0 || strcasecmp(_req_head_para._method.c_str(), "HEAD") == 0)
			{
				result = 1;
				ret = len;
			}
			else
			{
				if (_boundary_para._boundary_str.length() == 0)
				{
					ret = http_base_process<DATA_PROCESS>::_data_process->process_recv_body(buf, len, result);
					_recv_body_length += ret;
					if (_recv_body_length == _req_head_para._content_length)
					{
						result = 1;
					}				
				}
				else //parse boundary
				{
					ret = get_boundary(buf, len, result);
				}
			}
			return ret;
		}
		
        void parse_header()
        {
            vector<string>& strList;
            SplitString(head_str, "\r\n", strList);
            for (int i = 0; i < strList.size(); i++) {
                if (!i) {
                    vector<string>& str1List;
                    SplitString(strList[i], " ", str1List);
                    if ()

                }
            }
        }

        void parse_header()
        {
            string &head_str = http_base_process<DATA_PROCESS>::_recv_head;
            //parse method
            int ret = GetStringByLabel(head_str, "", " ", _req_head_para._method);
            if (ret != 0)
            {            	
                THROW_COMMON_EXCEPT("http get method fail")
            }
			
            //parse url
            string s_path;
            ret = GetStringByLabel(head_str, " ", " ", s_path);
            if (ret != 0)
            {
                THROW_COMMON_EXCEPT("http get url fail")
            }

            size_t pos = s_path.find("?");
            if (pos == string::npos)
            {
                _req_head_para._url_path = s_path;
            }
            else
            {
                _req_head_para._url_path = s_path.substr(0, pos);       
				string para_str = s_path.substr(pos + 1);
				http_base_process<DATA_PROCESS>::parse_url_para(para_str, _req_head_para._url_para_list);
            }
            //parse cookie			
            string cookie_str;
            ret = CToolKit::GetCaseStringByLabel(head_str, "Cookie:", "\r\n", cookie_str);
            if (ret == 0)
            {
                vector<string> cookie_vec;
                CToolKit::SplitString(cookie_str, ";", cookie_vec);
                size_t c_num = cookie_vec.size();
                for (size_t ii = 0; ii < c_num; ii++)
                {
                    vector<string> c_tmp_vec;
                    CToolKit::SplitString(cookie_vec[ii], "=", c_tmp_vec);
                    if (c_tmp_vec.size() == 2)
                    {
                        CToolKit::StringTrim(c_tmp_vec[0]);
                        CToolKit::StringTrim(c_tmp_vec[1]);
                        _req_head_para._cookie_list.insert(make_pair(c_tmp_vec[0], c_tmp_vec[1]));
                    }
                }
            }

			
			string s_tmp;
			if (_req_head_para._method == "POST" || _req_head_para._method == "PUT")
			{
	            //parse content_length
	            ret = GetCaseStringByLabel(head_str, "Content-length:", "\r\n", s_tmp);
	            if (ret == 0)
	            {
	                _req_head_para._content_length = strtoull(s_tmp.c_str(), 0, 10);
	            }

				//parse content_type
				ret = CToolKit::GetCaseStringByLabel(head_str, "Content-Type:", "\r\n", s_tmp);
				if (ret == 0)
				{
					StringTrim(s_tmp);
					_req_head_para._content_type = s_tmp;

				    if (strncasestr(s_tmp.c_str(), s_tmp.length(),  "multipart/form-data") != NULL)
				    {
					   	ret = GetCaseStringByLabel(s_tmp, "boundary=", "", _boundary_para._boundary_str);						
				    }
				}
		    }

			//parse connection			
			ret = CToolKit::GetCaseStringByLabel(head_str, "Connection:", "\r\n", s_tmp);
			if (ret == 0)
			{
				_req_head_para._connect_type = strtoull(s_tmp.c_str(), 0, 10);
				StringTrim(_req_head_para._connect_type);
			}
            //parse host
            ret = GetCaseStringByLabel(head_str, "Host:", "\r\n", s_tmp);
            if (ret == 0)
            {
                CToolKit::StringTrim(s_tmp);
                _req_head_para._host = s_tmp;
            }            
        }

		
        void gen_send_head()
        {       				
            http_base_process<DATA_PROCESS>::_send_head = http_base_process<DATA_PROCESS>::_data_process->gen_send_head();
        }

        void recv_finish()
        {
            //http_base_process<DATA_PROCESS>::_http_status = SEND_HEAD;
            http_base_process<DATA_PROCESS>::_data_process->recv_finish();
            http_base_process<DATA_PROCESS>::change_http_status(SEND_HEAD);
        }

        void send_finish()
        {        	
        	if (strcasecmp(_req_head_para._connect_type.c_str(), "Close") == 0)
    		{    			
    		}
			else
			{
				reset();
			}
        }


		size_t get_boundary(char *buf, size_t len, int &result)
		{	
			if (_req_head_para._content_length == (uint32_t)-1)
			{
				THROW_COMMON_EXCEPT(-1, "get boundary but content_len not found")
			}
			size_t ret = len;
			size_t p_len = 0;
			result = 0;
			_recv_body_length += len;
			//首先要找第一个boundary
			if (_recv_boundary_status == BOUNDARY_RECV_HEAD)
			{				
				_recv_boundary_head.append(buf, len);
				size_t pos = _recv_boundary_head.find("\r\n\r\n");
				if (pos != string::npos)
				{					
					_boundary_para._boundary_content_length = _req_head_para._content_length - (_boundary_para._boundary_str.length() + BOUNDARY_EXTRA_LEN)
							- (pos+4);	
					string left_str;
					if (_recv_body_length == _req_head_para._content_length)
					{											
						left_str = _recv_boundary_head.substr(pos+4, _boundary_para._boundary_content_length);
					}
					else if (_recv_body_length >= _req_head_para._content_length - (_boundary_para._boundary_str.length() + BOUNDARY_EXTRA_LEN)) 
					{
						left_str = _recv_boundary_head.substr(pos+4, _boundary_para._boundary_content_length);
						_recv_boundary_status = BOUNDARY_RECV_TAIL;
					}
					else
					{
						left_str = _recv_boundary_head.substr(pos+4);
						_recv_boundary_status = BOUNDARY_RECV_BODY;
					}

					if (left_str.length() > 0)
					{
						p_len = http_base_process<DATA_PROCESS>::_data_process->process_recv_body(left_str.c_str(), 
							left_str.length(), result);
						if (_recv_body_length == _req_head_para._content_length)
							result = 1;
						p_len = left_str.length() - p_len;
					}
					else //什么也不干
					{
					}
				}
				else //还要继续收头
				{
					if (_recv_boundary_head.length() >= MAX_HTTP_HEAD_LEN)
						THROW_COMMON_EXCEPT(-1, "http boundary head too long (" << _recv_boundary_head.length() << ")")
				}
			}
			else if (_recv_boundary_status == BOUNDARY_RECV_BODY)//recv_body
			{		
				int tmp_len = len;
				if (_recv_body_length == _req_head_para._content_length)
				{
					tmp_len = len - (_boundary_para._boundary_str.length() + BOUNDARY_EXTRA_LEN);
					
				}
				else if (_recv_body_length >= _req_head_para._content_length - (_boundary_para._boundary_str.length() + BOUNDARY_EXTRA_LEN))
				{
					tmp_len = len - (_recv_body_length - (_req_head_para._content_length - (_boundary_para._boundary_str.length() + BOUNDARY_EXTRA_LEN)));
					_recv_boundary_status = BOUNDARY_RECV_TAIL;
				}
				else //还要收body
				{
				}
				
				p_len = http_base_process<DATA_PROCESS>::_data_process->process_recv_body(buf, tmp_len, result);
				p_len = tmp_len - p_len;
				
				if (_recv_body_length == _req_head_para._content_length)
					result = 1; //结束了
			}
			else //recv tail
			{	
				//什么也不干，只等收完
				if (_recv_body_length == _req_head_para._content_length)
					result = 1;				
			}
			_recv_body_length = _recv_body_length - p_len;
			return ret - p_len;
		}
      
    protected:
		enum BOUNDARY_STATUS
		{
			BOUNDARY_RECV_HEAD = 0,
			BOUNDARY_RECV_BODY = 1,
			BOUNDARY_RECV_TAIL = 2
		};
		string _recv_boundary_head;
		boundary_para _boundary_para;		
		BOUNDARY_STATUS _recv_boundary_status;
		uint32_t _recv_body_length;
		
        http_head_para _req_head_para;
        http_res_head_para _res_head_para;
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

template<class DATA_PROCESS>
class http_req_process:public http_base_process<DATA_PROCESS>
{
    public:
        http_req_process(NET_OBJ *p_connect):http_base_process<DATA_PROCESS>(p_connect)
	    {
	        //http_base_process<DATA_PROCESS>::_http_status = SEND_HEAD;
	         http_base_process<DATA_PROCESS>::change_http_status(SEND_HEAD);
			_cur_chunked_len = -1;
			_cur_chunked_rec_len = -1;
			_recv_body_length = 0;
			_p_send_arg = NULL;
			_recv_type = CONTENT_LENGTH_TYPE;
			http_base_process<DATA_PROCESS>::_data_process = DATA_PROCESS::gen_process(this);
	    }

        ~http_req_process()
        {
        	if (_p_send_arg != NULL)
				delete _p_send_arg;
        }

		
		string* get_send_buf()
		{			
			return http_base_process<DATA_PROCESS>::get_send_buf();
		}		

        http_head_para & get_req_head_para()
        {
            return _req_head_para;
        }

        http_res_head_para& get_res_head_para()
        {
            return _res_head_para;
        }

		void set_req_head_para(const http_head_para &para)
		{
			_req_head_para = para;
		}

	  	void reset()
        {
            http_base_process<DATA_PROCESS>::reset();
			http_base_process<DATA_PROCESS>::change_http_status(SEND_HEAD, false);
            _req_head_para.init();
            _res_head_para.init();

			_cur_chunked_len = -1;
			_cur_chunked_rec_len = -1;
			_recv_body_length = 0;
			_chunked_body.clear();

			
			http_base_process<DATA_PROCESS>::_data_process = DATA_PROCESS::gen_process(this);
        }

		connect_info* gen_connect_info()
		{
			http_connect_info *p_info = new http_connect_info();
			p_info->_head_para = _req_head_para;
			_p_http_connect_info = p_info;
			return p_info;
		}

		http_connect_info *get_connect_info()
		{
			return _p_http_connect_info;
		}

		void set_send_arg(http_send_arg *arg)
		{
			if (_p_send_arg != NULL)
				delete _p_send_arg;
			_p_send_arg = arg;
		}

		http_send_arg* get_send_arg()
		{
			return _p_send_arg;
		}

		void peer_close()
		{
			if (OTHER_TYPE == _recv_type)
			{
				recv_finish();
			}
		}

		string gen_req_head()
		{
 	        //method
            stringstream ss;
            ss << _req_head_para._method << " " << _req_head_para._url_path << " HTTP/1.1" << "\r\n";
            //content-length
            if (_req_head_para._content_length != (uint64_t)-1)
            {
                ss << "Content-Length: " << _req_head_para._content_length << "\r\n";
            }
            //cookie
            if (_req_head_para._cookie_list.size() > 0)
            {
                ss << "Cookie: ";
                int ii = 0;
                for (map<string, string>::iterator itr = _req_head_para._cookie_list.begin(); 
                        itr != _req_head_para._cookie_list.end(); ++itr)
                {
                    if (ii > 0)
                        ss << ";";
                    ss <<  itr->first << "=" << itr->second;
                    ii++;
                }
                ss << "\r\n";
            }

            ss << "Host: " << _req_head_para._host << "\r\n";

            //other para
            for (map<string, string>::iterator itr = _req_head_para._other_list.begin(); 
                    itr != _req_head_para._other_list.end(); ++itr)
            {
                ss << itr->first << ": " << itr->second << "\r\n";
            }
            //最后一个分隔符
            ss << "\r\n";
            return ss.str();
		}
    protected:
        size_t process_recv_body(char *buf, size_t len, int &result)
        {
        	result = 0;
        	size_t ret  = 0;
            if (_res_head_para._chunked == "chunked")
            {
            	_recv_type = CHUNK_TYPE;
                ret = get_chuncked(buf, len, result);				
            }
            else if (_res_head_para._content_length != (uint64_t)-1)
            {
            	_recv_type = CONTENT_LENGTH_TYPE;
                ret = http_req_process<DATA_PROCESS>::_data_process->process_recv_body(buf, len);
				_recv_body_length += ret;
				if (_recv_body_length == _res_head_para._content_length)
				{
					result = 1;
				}
            }
			else
			{
				_recv_type = OTHER_TYPE;
				//THROW_COMMON_EXCEPT(-1, "recv body fail " << http_base_process<DATA_PROCESS>::_recv_head.c_str());
				 ret = http_req_process<DATA_PROCESS>::_data_process->process_recv_body(buf, len);
				_recv_body_length += ret;
			}
			return ret;
        }

        size_t get_chuncked(char *buf, size_t len, int &result)
        {
            size_t ret = len;
			size_t p_len = 0;
			result = 0;
			if (len == 0)
				return 0;
			
            _chunked_body.append(buf, len);
            while (1)
            {
                if (_cur_chunked_len == -1)
                {   
                    string sTmp;
                    int nRet= CToolKit::GetStringByLabel(_chunked_body, "", "\r\n", sTmp, 0, 1);            
                    if (nRet != -1)
                    {
                        _cur_chunked_len=strtoul(sTmp.c_str(), 0, 16);
                        _cur_chunked_rec_len = nRet;
                        if(_cur_chunked_len == 0)
                        {         
                        	result = 1; //结束了
                            break;
                        }

                        if (_cur_chunked_len + 2 + _cur_chunked_rec_len  > (int)_chunked_body.length())
                        {
                            break;
                        }
                        else
                        {
                            p_len = http_req_process<DATA_PROCESS>::_data_process->process_recv_body((char*)_chunked_body.substr(_cur_chunked_rec_len, _cur_chunked_len).c_str(), _cur_chunked_len);
							p_len = _cur_chunked_len - p_len;
							
                            _chunked_body = _chunked_body.substr(_cur_chunked_rec_len+2+_cur_chunked_len);
                            _cur_chunked_len = -1;
                            if (0 == _chunked_body.length())
                                break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
                else //已经获取长度但是没有获取数据
                {          
                    if (_cur_chunked_len+2+_cur_chunked_rec_len > (int)_chunked_body.length())
                    {
                        break;
                    }
                    else
                    {
                        p_len = http_req_process<DATA_PROCESS>::_data_process->process_recv_body((char*)_chunked_body.substr(_cur_chunked_rec_len, _cur_chunked_len).c_str(), _cur_chunked_len); 
						p_len = _cur_chunked_len - p_len;
                        _chunked_body = _chunked_body.substr(_cur_chunked_len+_cur_chunked_rec_len+2);
                        _cur_chunked_len = -1;
                        if ( 0 == _chunked_body.length())
                            break;
                    }
                }
            }
            return ret - p_len;
        }
		

        void parse_header()
        {
            string recv_str = http_base_process<DATA_PROCESS>::_recv_head;		
            string s_tmp;
            //parse response code
            int ret = CToolKit::GetCaseStringByLabel(recv_str, " ", " ", s_tmp);
            if (ret == 0)
            {
                _res_head_para._response_code = strtoull(s_tmp.c_str(), 0, 10);
            }
            else
            {
                THROW_COMMON_EXCEPT(-1, "get http response code fail ret");
            }

            //parse content_length
            ret = CToolKit::GetCaseStringByLabel(recv_str, "Content-length:", "\r\n", s_tmp);
            if (ret == 0)
            {
                _res_head_para._content_length = strtoull(s_tmp.c_str(), 0, 10);
            }
			
            //parse cookie
            s_tmp.clear();
			/*
            ret = CToolKit::GetCaseStringByLabel(recv_str, "Set-Cookie:", "\r\n", s_tmp);
            if (ret == 0)
            {
                vector<string> cookie_vec;
                CToolKit::SplitString(s_tmp, ";", cookie_vec);
                size_t c_num = cookie_vec.size();
                for (size_t ii = 0; ii < c_num; ii++)
                {
                    vector<string> c_tmp_vec;
                    CToolKit::SplitString(cookie_vec[ii], "=", c_tmp_vec);
                    if (c_tmp_vec.size() == 2)
                    {
                        CToolKit::StringTrim(c_tmp_vec[0]);
                        CToolKit::StringTrim(c_tmp_vec[1]);
                        _res_head_para._cookie_list.insert(make_pair(c_tmp_vec[0], c_tmp_vec[1]));
                    }
                }
            }
            		*/

            //parse chunked
            ret = CToolKit::GetCaseStringByLabel(recv_str, "Transfer-Encoding:", "\r\n", _res_head_para._chunked);
            if (ret == 0)
            {
                CToolKit::StringTrim(_res_head_para._chunked);	
            }
        }

        virtual void gen_send_head()
        {             	
        	http_base_process<DATA_PROCESS>::_send_head = http_base_process<DATA_PROCESS>::_data_process->gen_send_head();
        }      

        void recv_finish()
        {        	
	     	http_base_process<DATA_PROCESS>::_data_process->recv_finish();            
			reset();
        }

        void send_finish()
        {
            //http_base_process<DATA_PROCESS>::_http_status = RECV_HEAD;
            http_base_process<DATA_PROCESS>::change_http_status(RECV_HEAD);
        }


    protected:
        http_head_para _req_head_para;
        http_res_head_para _res_head_para;

		int64_t _cur_chunked_len;
		int64_t _cur_chunked_rec_len;
		string _chunked_body;
		size_t _recv_body_length;
		http_send_arg* _p_send_arg;

		HTTP_RECV_TYPE _recv_type;

		http_connect_info *_p_http_connect_info;
};


//每一个http_data_process 需要实现下列7个函数即可
/*
class http_base_data_process
{
	public:
		//result:0 正常返回  1: 结束获取发送包， 一般情况下上层自动会判断
		//return NULL 底层就删除写事件, 正常返回将要发送的包体
		string *get_send_body(int &result);
		
		//头部接收完处理
		void header_recv_finish(); 
		

		//所有的数据接收完处理
		void recv_finish();
		

		//生成发送头
       	string gen_send_head();      	
       	

		//处理接收的数据
		size_t process_recv_body(const char *buf, size_t len, int& result);

		//处理外部消息 一般这个没啥用了
		void process_s(normal_obj_msg *p_msg);


		//对象生成器
		static http_base_data_process *gen_process(http_res_process<http_base_data_process> *p);		
};
*/
#endif

