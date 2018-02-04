#include "base_def.h"
#include "http_req_process.h"
#include "base_singleton.h"
#include "http_base_process.h"
#include "common_exception.h"
#include "common_def.h"
#include "log_helper.h"
#include "http_base_data_process.h"


http_req_process::http_req_process(base_net_obj *p):http_base_process(p)
{
    http_base_process::change_http_status(SEND_HEAD);
    _cur_chunked_len = -1;
    _cur_chunked_rec_len = -1;
    _recv_body_length = 0;
    _recv_type = CONTENT_LENGTH_TYPE;
}

http_req_process::~http_req_process()
{
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


void http_req_process::peer_close()
{
    if (OTHER_TYPE == _recv_type)
    {
        recv_finish();
    }
}

void http_req_process::parse_first_line(const string & line)
{
    vector<string> tmp_vec;
    SplitString(line.c_str(), " ", &tmp_vec);
    if (tmp_vec.size() != 3) {
        THROW_COMMON_EXCEPT("http first line");
    }
    _res_head_para._response_code = strtoull(tmp_vec[1].c_str(), 0, 10);
    _res_head_para._version = tmp_vec[0];
}

void http_req_process::parse_header(string & recv_head)
{
    string &head_str = recv_head;
    vector<string> strList;
    SplitString(head_str.c_str(), CRLF, &strList);
    for (uint32_t i = 0; i < strList.size(); i++) {
        if (!i) {
            parse_first_line(strList[i]);
        }else {
            vector<string> tmp_vec;
            SplitString(strList[i].c_str(), ":", &tmp_vec);
            if (tmp_vec.size() != 2) {
                THROW_COMMON_EXCEPT("http headers parms error");
            } else {
                _res_head_para._headers.insert(make_pair(tmp_vec[0], tmp_vec[1]));
                LOG_DEBUG("%s: %s", tmp_vec[0].c_str(), tmp_vec[1].c_str());
            }
        }
    }

    //parse chunked
    string * tmp_str= _res_head_para.get_header("Transfer-Encoding:");
    if (tmp_str)
    {	
        _res_head_para._chunked = *tmp_str;
        StringTrim(_res_head_para._chunked);	
    }

}

size_t http_req_process::process_recv_body(const char *buf, size_t len, int &result)
{
    result = 0;
    size_t ret  = 0;
    uint64_t content_length = 0;
    string *tmp_str = _res_head_para.get_header("Content-Length");
    if (tmp_str) 
    {
        content_length = strtoull(tmp_str->c_str(), 0, 10);
    }
    if (_res_head_para._chunked == "chunked")
    {
        _recv_type = CHUNK_TYPE;
        ret = get_chuncked(buf, len, result);				
    }
    else if (content_length)
    {
        _recv_type = CONTENT_LENGTH_TYPE;
        ret = _data_process->process_recv_body(buf, len, result);
        _recv_body_length += ret;
        if (_recv_body_length == content_length)
        {
            result = 1;
        }
    }
    else
    {
        _recv_type = OTHER_TYPE;
        //THROW_COMMON_EXCEPT(-1, "recv body fail " << http_base_process<DATA_PROCESS>::_recv_head.c_str());
        ret = _data_process->process_recv_body(buf, len, result);
        _recv_body_length += ret;
    }
    return ret;
}

size_t http_req_process::get_chuncked(const char *buf, size_t len, int &result)
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
                    result = 1; //结束了
                    break;
                }

                if (_cur_chunked_len + 2 + _cur_chunked_rec_len  > (int)_chunked_body.length())
                {
                    break;
                }
                else
                {
                    p_len = _data_process->process_recv_body((char*)_chunked_body.substr(_cur_chunked_rec_len, _cur_chunked_len).c_str(), _cur_chunked_len, result);
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
                p_len = _data_process->process_recv_body((char*)_chunked_body.substr(_cur_chunked_rec_len, _cur_chunked_len).c_str(), _cur_chunked_len, result); 
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

void http_req_process::recv_finish()
{        	
    _data_process->msg_recv_finish();            
    reset();
}

void http_req_process::send_finish()
{
    //http_base_process<DATA_PROCESS>::_http_status = RECV_HEAD;
    change_http_status(RECV_HEAD);
}


