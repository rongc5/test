#include "base_def.h"
#include "http_req_process.h"
#include "base_singleton.h"
#include "http_base_process.h"
#include "common_exception.h"
#include "common_def.h"
#include "log_helper.h"
#include "base_connect.h"


http_req_process::http_req_process(base_connect *p):http_base_process(p)
{
    http_base_process::change_http_status(SEND_HEAD);
    _cur_chunked_len = -1;
    _cur_chunked_rec_len = -1;
    _recv_body_length = 0;
    _p_send_arg = NULL;
    _recv_type = CONTENT_LENGTH_TYPE;
}

http_req_process::~http_req_process()
{
    if (_p_send_arg != NULL)
        delete _p_send_arg;
}


string* http_req_process::get_send_buf()
{			
    return http_base_process::get_send_buf();
}		

http_head_para & http_req_process::get_req_head_para()
{
    return _req_head_para;
}

http_res_head_para& http_req_process::get_res_head_para()
{
    return _res_head_para;
}

void http_req_process::set_req_head_para(const http_head_para &para)
{
    _req_head_para = para;
}

void http_req_process::reset()
{
    http_base_process::reset();
    http_base_process::change_http_status(SEND_HEAD, false);
    _req_head_para.init();
    _res_head_para.init();

    _cur_chunked_len = -1;
    _cur_chunked_rec_len = -1;
    _recv_body_length = 0;
    _chunked_body.clear();
}


void http_req_process::set_send_arg(http_send_arg *arg)
{
    if (_p_send_arg != NULL)
        delete _p_send_arg;
    _p_send_arg = arg;
}

http_send_arg* http_req_process::get_send_arg()
{
    return _p_send_arg;
}

void http_req_process::peer_close()
{
    if (OTHER_TYPE == _recv_type)
    {
        recv_finish();
    }
}

string http_req_process::gen_req_head()
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
    //���һ���ָ���
    ss << "\r\n";
    return ss.str();
}

size_t http_req_process::process_recv_body(char *buf, size_t len, int &result)
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
        ret = _data_process->process_recv_body(buf, len);
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
        ret = _data_process->process_recv_body(buf, len);
        _recv_body_length += ret;
    }
    return ret;
}

size_t http_req_process::get_chuncked(char *buf, size_t len, int &result)
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
            int nRet= GetStringByLabel(_chunked_body, "", "\r\n", sTmp, 0, 1);            
            if (nRet != -1)
            {
                _cur_chunked_len=strtoul(sTmp.c_str(), 0, 16);
                _cur_chunked_rec_len = nRet;
                if(_cur_chunked_len == 0)
                {         
                    result = 1; //������
                    break;
                }

                if (_cur_chunked_len + 2 + _cur_chunked_rec_len  > (int)_chunked_body.length())
                {
                    break;
                }
                else
                {
                    p_len = _data_process->process_recv_body((char*)_chunked_body.substr(_cur_chunked_rec_len, _cur_chunked_len).c_str(), _cur_chunked_len);
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
        else //�Ѿ���ȡ���ȵ���û�л�ȡ����
        {          
            if (_cur_chunked_len+2+_cur_chunked_rec_len > (int)_chunked_body.length())
            {
                break;
            }
            else
            {
                p_len = _data_process->process_recv_body((char*)_chunked_body.substr(_cur_chunked_rec_len, _cur_chunked_len).c_str(), _cur_chunked_len); 
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


void http_req_head_para::parse_header()
{
    string recv_str = ::_recv_head;
    string s_tmp;
    //parse response code
    int ret = GetCaseStringByLabel(recv_str, " ", " ", s_tmp);
    if (ret == 0)
    {
        _res_head_para._response_code = strtoull(s_tmp.c_str(), 0, 10);
    }
    else
    {
        THROW_COMMON_EXCEPT("get http response code fail ret");
    }

    //parse content_length
    ret = GetCaseStringByLabel(recv_str, "Content-length:", "\r\n", s_tmp);
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
    ret = GetCaseStringByLabel(recv_str, "Transfer-Encoding:", "\r\n", _res_head_para._chunked);
    if (ret == 0)
    {
        StringTrim(_res_head_para._chunked);	
    }
}

void http_req_process::gen_send_head()
{             	
    _send_head = _data_process->gen_send_head();
}      

void http_req_process::recv_finish()
{        	
    _data_process->recv_finish();            
    reset();
}

void http_req_process::send_finish()
{
    //http_base_process<DATA_PROCESS>::_http_status = RECV_HEAD;
    change_http_status(RECV_HEAD);
}

