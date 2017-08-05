#include "base_def.h"
#include "base_connect.h"
#include "base_singleton.h"
#include "http_res_process.h"
#include "common_exception.h"
#include "common_def.h"
#include "log_helper.h"
#include "http_base_process.h"
#include "http_base_data_process.h"


http_res_process::http_res_process(base_connect * p):http_base_process(p)
{
    change_http_status(RECV_HEAD);
    _recv_body_length = 0;
    _recv_boundary_status = BOUNDARY_RECV_HEAD;
}

http_res_process::~http_res_process()
{		
}


void http_res_process::reset()
{
    http_base_process::reset();
    change_http_status(RECV_HEAD);

    _boundary_para.init();
    _recv_body_length = 0;
    _recv_boundary_status = BOUNDARY_RECV_HEAD;
}

void http_res_process::gen_send_head(string * head)
{
    if (!head) {
        return;
    }
    //返回状态码
    string response_str = _res_code.get_response_str(_res_head_para._response_code);

    {
        head->append(_req_head_para._version);
        head->append(" ");
        char tmp_buf[SIZE_LEN_32];
        snprintf(tmp_buf, sizeof(tmp_buf), "%d", _res_head_para._response_code);
        head->append(tmp_buf);
        head->append(" ");
        head->append(response_str);
        head->append(CRLF);
    }

    //cookie
    if (_res_head_para._cookie_list.size() > 0)
    {
        for (map<string, set_cookie_item>::iterator itr = _res_head_para._cookie_list.begin(); 
                itr != _res_head_para._cookie_list.end(); ++itr)
        {
            head->append("Set-Cookie: ");
            head->append(itr->first);
            head->append("=");
            head->append(itr->second._value);
            if (itr->second._expire != 0)
            {
                head->append(";expires=");
                head->append(SecToHttpTime(itr->second._expire));
            }

            if (itr->second._path != "")
            {
                head->append(";path=");
                head->append(itr->second._path);
            }

            if (itr->second._domain != "")
            {
                head->append(";domain=");
                head->append(itr->second._domain);
            }
            head->append(CRLF);
        }
    }

    //content_length
    if (_res_head_para._content_length != (uint64_t)-1)
    {
        head->append("Content-Length: ");
        char tmp_buf[SIZE_LEN_32];
        snprintf(tmp_buf, sizeof(tmp_buf), "%llu", _res_head_para._content_length);
        head->append(tmp_buf);
        head->append(CRLF);
    }

    //other para
    for (map<string, string>::iterator itr = _res_head_para._headers.begin(); 
            itr != _res_head_para._headers.end(); ++itr)
    {
        head->append(itr->first);
        head->append(": ");
        head->append(itr->second);
        head->append(CRLF);
    }
    //最后一个分隔符
    head->append(CRLF);
}

size_t http_res_process::process_recv_body(const char *buf, size_t len, int &result)
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
    vector<string> tmp_vec;
    SplitString(line, " ", tmp_vec);
    if (tmp_vec.size() != 3) {
        THROW_COMMON_EXCEPT("http first line");
    }
    _req_head_para._method = tmp_vec[0];
    _req_head_para._version = tmp_vec[2];
    size_t pos = tmp_vec[1].find("?");
    if (pos == string::npos)
    {
        _req_head_para._url_path = tmp_vec[1];
    }
    else
    {
        _req_head_para._url_path = tmp_vec[1].substr(0, pos);       
        string para_str = tmp_vec[1].substr(pos + 1);
        parse_url_para(para_str, _req_head_para._url_para_list);
    }
}

void http_res_process::parse_url_para(const string &url_para, map<string, string> &url_para_map)
{
    vector<string> vec_str;
    SplitString(url_para, "&", vec_str);
    size_t num = vec_str.size();
    for (size_t ii = 0; ii < num; ii ++)
    {
        vector<string> tmp_vec;
        SplitString(vec_str[ii], "=", tmp_vec);
        if (tmp_vec.size() == 2)
        {
            StringTrim(tmp_vec[0]);
            StringTrim(tmp_vec[1]);
            string tmp_para;
            UrlDecode(tmp_vec[1], tmp_para);
            url_para_map.insert(make_pair(tmp_vec[0], tmp_para));
        }
    }               
}


void http_res_process::parse_header(string & recv_head)
{
    string &head_str = recv_head;
    vector<string> strList;
    SplitString(head_str, CRLF, strList);
    for (uint32_t i = 0; i < strList.size(); i++) {
        if (!i) {
            parse_first_line(strList[i]);
        }else {
            vector<string> tmp_vec;
            SplitfirstDelimString(strList[i], ":", tmp_vec);
            if (tmp_vec.size() != 2) {
                THROW_COMMON_EXCEPT("http headers parms error");
            } else {
                _req_head_para._headers.insert(make_pair(tmp_vec[0], tmp_vec[1]));
                LOG_DEBUG("%s: %s", tmp_vec[0].c_str(), tmp_vec[1].c_str());
            }
        }
    }

    string * cookie_str = _req_head_para.get_header("Cookie");
    if (cookie_str)
    {
        vector<string> cookie_vec;
        SplitString(*cookie_str, ";", cookie_vec);
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
        tmp_str = _req_head_para.get_header("Content-Length");
        if (tmp_str)
        {
            _req_head_para._content_length = strtoull(tmp_str->c_str(), 0, 10);
        }

        //parse content_type
        tmp_str = _req_head_para.get_header("Content-Type");
        if (tmp_str)
        {
            _req_head_para._content_type = *tmp_str;

            if (strncasestr(tmp_str->c_str(), tmp_str->length(),  "multipart/form-data") != NULL)
            {
                GetCaseStringByLabel(*tmp_str, "boundary=", "", _boundary_para._boundary_str);
            }
        }
    }

    //parse connection		

    tmp_str = _req_head_para.get_header("connection");
    if (tmp_str)
    {
        _req_head_para._connect_type = *tmp_str;
        StringTrim(_req_head_para._connect_type);
    }
    tmp_str = _req_head_para.get_header("Host");
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


size_t http_res_process::get_boundary(const char *buf, size_t len, int &result)
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



