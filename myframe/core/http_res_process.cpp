#include "base_def.h"
#include "base_connect.h"
#include "base_singleton.h"
#include "http_res_process.h"
#include "common_exception.h"
#include "common_def.h"
#include "log_helper.h"


http_res_process::http_res_process(base_connect * p):http_base_process(p)
{
    change_http_status(RECV_HEAD);
    _recv_body_length = 0;
    _recv_boundary_status = BOUNDARY_RECV_HEAD;
}

http_res_process::~http_res_process()
{		
}

http_head_para &http_res_process::get_req_head_para()
{
    return _req_head_para;
}

http_res_head_para &http_res_process::get_res_head_para()
{
    return _res_head_para;
}

void http_res_process::set_res_head_para(const http_res_head_para &para)
{
    _res_head_para = para;
}

void http_res_process::reset()
{
    http_base_process::reset();        
    change_http_status(RECV_HEAD);
    _req_head_para.init();
    _res_head_para.init();

    _boundary_para.init();
    _recv_body_length = 0;
    _recv_boundary_status = BOUNDARY_RECV_HEAD;
}

string http_res_process::gen_res_head()
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

size_t http_res_process::process_recv_body(char *buf, size_t len, int &result)
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
            ret = _data_process->process_recv_body(buf, len, result);
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


void http_res_process::parse_first_line(const string & line)
{
    vector<string>& tmp_vec;
    SplitString(line, " ", tmp_vec);
    if (tmp_vec.size() != 3) {
        THROW_COMMON_EXCEPT("http first line");
    }
    _req_head_para._method = tmp_vec[0];
    size_t pos = tmp_vec[1].find("?");
    if (pos == string::npos)
    {
        _req_head_para._url_path = s_path;
    }
    else
    {
        _req_head_para._url_path = s_path.substr(0, pos);       
        string para_str = s_path.substr(pos + 1);
        parse_url_para(para_str, _req_head_para._url_para_list);
    }
}


void http_res_process::parse_header()
{
    string &head_str = _recv_head;
    vector<string>& strList;
    SplitString(head_str, "\r\n", strList);
    for (int i = 0; i < strList.size(); i++) {
        if (!i) {
            parse_first_line(strList[i]);
        }else {
            vector<string>& tmp_vec;
            SplitfirstDelimString(strList[i], ":", tmp_vec);
            if (tmp_vec.size() != 2) {
                THROW_COMMON_EXCEPT("http headers parms error");
            } else {
                _headers[StringTrim(tmp_vec[0])] = StringTrim(tmp_vec[1]);
            }
        }
    }

    string * cookie_str = get_header("Cookie");
    if (cookie_str)
    {
        vector<string> cookie_vec;
        SplitString(cookie_str, ";", cookie_vec);
        size_t c_num = cookie_vec.size();
        for (size_t ii = 0; ii < c_num; ii++)
        {
            vector<string> c_tmp_vec;
            SplitString(cookie_vec[ii], "=", c_tmp_vec);
            if (c_tmp_vec.size() == 2)
            {
                StringTrim(c_tmp_vec[0]);
                StringTrim(c_tmp_vec[1]);
                _req_head_para._cookie_list.insert(make_pair(c_tmp_vec[0], c_tmp_vec[1]));
            }
        }
    }


    string *tmp_str = NULL;
    if (_req_head_para._method == "POST" || _req_head_para._method == "PUT")
    {
        //parse content_length
        tmp_str = get_header("content_length");
        if (tmp_str)
        {
            _req_head_para._content_length = strtoull(tmp_str->c_str(), 0, 10);
        }

        //parse content_type
        tmp_str = get_header("content_type");
        ret = GetCaseStringByLabel(head_str, "Content-Type:", "\r\n", s_tmp);
        if (tmp_str)
        {
            _req_head_para._content_type = *tmp_str;

            if (strncasestr(tmp_str->c_str(), tmp_str->length(),  "multipart/form-data") != NULL)
            {
                ret = GetCaseStringByLabel(*tmp_str, "boundary=", "", _boundary_para._boundary_str);
            }
        }
    }

    //parse connection		

    tmp_str = get_header("connection");
    if (tmp_str)
    {
        _req_head_para._connect_type = *tmp_str;
        StringTrim(_req_head_para._connect_type);
    }
    tmp_str = get_header("Host");
    if (tmp_str)
    {
        _req_head_para._host = *tmp_str;
    }            
}


void http_res_process::recv_finish()
{
    _data_process->msg_recv_finish();
    change_http_status(SEND_HEAD);
}

void http_res_process::send_finish()
{        	
    reset();
}


size_t http_res_process::get_boundary(char *buf, size_t len, int &result)
{	
    if (_req_head_para._content_length == (uint32_t)-1)
    {
        THROW_COMMON_EXCEPT("get boundary but content_len not found");
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
                p_len = _data_process->process_recv_body(left_str.c_str(), left_str.length(), result);
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
                THROW_COMMON_EXCEPT("http boundary head too long (" << _recv_boundary_head.length() << ")");
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

        p_len = _data_process->process_recv_body(buf, tmp_len, result);
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



