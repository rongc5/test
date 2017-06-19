#ifndef __HTTP_DATA_PROCESS_H__
#define __HTTP_DATA_PROCESS_H__

#include "common_def.h"
#include "http_data_process.h"
#include "base_data_process.h"



http_data_process::http_data_process(tcp_connect * t_cn);
{
    _send_head_len = 0;
    //_data_process = NULL;
    _p_connect = p;
    _send_status = 0;			
}

http_data_process::~http_base_process()
{
    if (_data_process != NULL)
        delete _data_process;
}

size_t http_data_process::process_recv_buf(char *buf, const size_t len)
{	
    if (_http_status > RECV_BODY)
    {
        THROW_COMMON_EXCEPT(-1, "http recv status not correct " << _http_status)
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


string* http_data_process::get_send_buf()
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


void http_data_process::reset()
{						
    _send_head_len = 0;
    if (_data_process != NULL)
    {
        delete _data_process;
        _data_process = NULL;
    }
    _recv_head.clear();
}



/****************************以上是五个口子，以下是供底层调用********************************************/
string &http_data_process::get_head()
{
    return _recv_head;
}



void http_data_process::change_http_status(HTTP_STATUS status, bool if_change_send = true)
{
    _http_status = status;
    if (status == SEND_HEAD && if_change_send)
    {
        change_to_cansend();
    }
}


void http_data_process::change_to_cansend()
{
    if (_send_status == 0)
    {
        _send_status = 1;
        //_p_connect->add_event(EPOLLOUT);
        notice_send();
    }
}


void http_data_process::parse_url_para(const string &url_para, map<string, string> &url_para_map)
{
    vector<string> vec_str;
    CToolKit::SplitString(url_para, "&", vec_str);
    size_t num = vec_str.size();
    for (size_t ii = 0; ii < num; ii ++)
    {
        vector<string> tmp_vec;
        SplitString(vec_str[ii], "=", tmp_vec);
        if (tmp_vec.size() == 2)
        {
            CToolKit::StringTrim(tmp_vec[0]);
            CToolKit::StringTrim(tmp_vec[1]);
            string tmp_para;
            UrlDecode(tmp_vec[1], tmp_para);
            url_para_map.insert(make_pair(tmp_vec[0], tmp_para));
        }
    }				
}	


void http_data_process::check_head_finish(string &left_str)
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
            THROW_COMMON_EXCEPT(-1, "http head too long (" << _recv_head.length() << ")")
        }
    }			
}


