#include "rquotation_data_process.h"
#include "http_base_process.h"
#include "http_base_data_process.h"
#include "log_helper.h"
#include "out_connect.h"
#include "base_net_container.h"
#include "base_net_thread.h"

#include "base_def.h"
#include "http_req_process.h"


rquotation_data_process::rquotation_data_process(http_base_process * _p_process):http_base_data_process(_p_process)
{
}

std::string * rquotation_data_process::get_send_body(int &result)
{
    result = 1;

    if (!_req_msg.post_data.empty())
    {
        return new std::string(_req_msg.post_data);
    }

    return NULL;
}

void rquotation_data_process::set_req_msg(http_req_msg & req_msg)
{
    _req_msg = req_msg;
}


url_info & rquotation_data_process::get_url_info()
{
    return _url_info;
}


void rquotation_data_process::set_url_info(url_info & info)
{
    _url_info = info;
}

void rquotation_data_process::header_recv_finish()
{
    //base_net_container * net_container = get_base_net()->get_net_container();
    http_res_head_para & res_head_para = _base_process->get_res_head_para();
    std::string * str = new std::string;
    res_head_para.to_head_str(str);
    LOG_DEBUG("header: %s", str->c_str());

    std::shared_ptr<std::string> rc(str);
}

void rquotation_data_process::msg_recv_finish()
{
    LOG_DEBUG("recv_buf: %s", _recv_buf.c_str());

    _recv_buf.clear();
}

std::string * rquotation_data_process::get_send_head()
{
    http_req_head_para & req_head = _base_process->get_req_head_para();

    req_head._headers = _req_msg.headers;
    
    if (!_req_msg.sid.empty()) 
    {
        LOG_DEBUG("sid: %s", _req_msg.sid.c_str());
    }

    if (_req_msg.cmd_type == HTTP_REQ_GET)
    {
        req_head._method = "GET";
    }
    else if (_req_msg.cmd_type == HTTP_REQ_POST)
    {
        req_head._method = "POST";
    }


    req_head._url_path = _req_msg.url;
    req_head._version = "HTTP/1.1";

    req_head._headers.insert(std::make_pair("Host", _url_info.domain));
    if (!_req_msg.post_data.empty())
    {
        req_head._headers.insert(std::make_pair("content-length", _req_msg.post_data));
    }

    if (req_head.get_header("Accept"))
    {
        req_head._headers.insert(std::make_pair("Accept", "*/*"));
    }

    std::string * str = new std::string;
    req_head.to_head_str(str);
    LOG_DEBUG("%s", str->c_str());

    return str;
}

size_t rquotation_data_process::process_recv_body(const char *buf, size_t len, int& result)
{
    result = 1;

    _recv_buf.append(buf, len);

    //LOG_DEBUG("recv_buf: %s", _recv_buf.c_str());
    return len;
}

base_net_obj * rquotation_data_process::gen_net_obj(url_info & info, http_req_msg & req_msg)
{
    out_connect<http_req_process> * connect = new out_connect<http_req_process>(info.ip, info.port);
    http_req_process * req_process = new http_req_process(connect);
    rquotation_data_process * sk_process = new rquotation_data_process(req_process);
    req_process->set_process(sk_process);
    connect->set_process(req_process);
    connect->connect();

    sk_process->set_url_info(info);
    sk_process->set_req_msg(req_msg);
    if (req_msg.time_out)
    {
        timer_msg  t_msg;
        t_msg._time_length = req_msg.time_out;
        t_msg._timer_type = TIMER_TYPE_HTTP_REQ; 
        sk_process->add_timer(t_msg);
    }

    return connect;
}

bool rquotation_data_process::handle_timeout(timer_msg & t_msg)
{
    if (t_msg._timer_type == TIMER_TYPE_HTTP_REQ)
    {

    }
}

bool rquotation_data_process::process_recv_msg(ObjId & id, std::shared_ptr<normal_msg> & p_msg)
{
    return true;
}
