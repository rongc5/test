#include "http_base_process.h"
#include "base_connect.h"
#include "common_exception.h"
#include "http_base_data_process.h"
#include "log_helper.h"

http_base_process::http_base_process(base_net_obj *p):base_data_process(p)
{
    _data_process = NULL;
}

http_base_process::~http_base_process()
{
    if (_data_process != NULL)
        delete _data_process;
}

http_req_head_para &http_base_process::get_req_head_para()
{
    return _req_head_para;
}

http_res_head_para &http_base_process::get_res_head_para()
{
    return _res_head_para;
}

void http_base_process::reset()
{
    _req_head_para.init();
    _res_head_para.init();
}

void http_base_process::set_process(http_base_data_process * data_process)
{
    if (_data_process != NULL && _data_process != data_process) {
        delete _data_process;
        _data_process = NULL;
    }
    _data_process = data_process;
}

size_t http_base_process::process_recv_buf(const char *buf, size_t len)
{	
    if (_http_status > RECV_BODY)
    {
        THROW_COMMON_EXCEPT("http recv status not correct " << _http_status);
    }


    size_t ret = 0;
    bool staus_change = false;
    std::string left_str, recv_head;
    if (_http_status == RECV_HEAD && strstr(buf, CRLF2))
    {
        recv_head.append(buf, len);
        check_head_finish(recv_head, left_str);
        staus_change = true;				
    }

    if (get_base_net()->_msg_op != MSG_CONNECT) {
        return ret;
    }
    LOG_DEBUG("%s\n", buf);

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

std::string* http_base_process::get_send_buf()
{
    if (_http_status < SEND_HEAD)
    {
        LOG_WARNING("http send status not correct (%d)", _http_status);
        return NULL;
    }

    std::string *ret_str = NULL;
    if (_http_status == SEND_HEAD)
    {
        ret_str = _data_process->get_send_head();

        if (!ret_str)
            return NULL;

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

    return ret_str;
}

bool http_base_process::process_recv_msg(ObjId & id, std::shared_ptr<normal_msg> & p_msg)
{
    return _data_process->process_recv_msg(id, p_msg);
}

/****************************以上是五个口子，以下是供底层调用********************************************/

void http_base_process::change_http_status(HTTP_STATUS status, bool if_change_send)
{
    _http_status = status;
    if (status == SEND_HEAD && if_change_send)
    {
        _p_connect->notice_send();
    }
}

http_base_data_process * http_base_process::get_process()
{
    return _data_process;
}

void http_base_process::check_head_finish(std::string & recv_head, std::string &left_str)
{
    size_t pos =  recv_head.find(CRLF2);
    if (pos != std::string::npos)
    {
        left_str = recv_head.substr(pos + 4);
        recv_head.erase(pos + 4);				
        //_http_status = RECV_BODY;
        change_http_status(RECV_BODY);
        parse_header(recv_head);

        _data_process->header_recv_finish();
    }
    else
    {
        if (recv_head.length() > MAX_HTTP_HEAD_LEN) //http头不要超过10k
        {
            THROW_COMMON_EXCEPT("http head too long (" << recv_head.length() << ")");
        }
    }			
}




