#include "skhttp_req_data_process.h"
#include "http_base_process.h"
#include "http_base_data_process.h"
#include "log_helper.h"
#include "out_connect.h"
#include "base_net_container.h"
#include "base_net_thread.h"

#include "base_def.h"
#include "http_req_process.h"


skhttp_req_data_process::sah_data_process(http_base_process * _p_process):http_base_data_process(_p_process)
{
    _req_msg = NULL;
}

string * skhttp_req_data_process::get_send_body(int &result)
{
    result = 1;

    //string * body = new string(_body);

    //_body.clear();

    if (_req_msg.post_data)

    return NULL;
}

void skhttp_req_data_process::set_req_msg(http_req_msg * req_msg)
{
    _req_msg = req_msg;
}


url_info & skhttp_req_data_process::get_url_info()
{
    return _url_info;
}


void skhttp_req_data_process::set_url_info(url_info & info)
{
    _url_info = info;
}


void skhttp_req_data_process::header_recv_finish()
{
    //base_net_container * net_container = get_base_net()->get_net_container();
}

void skhttp_req_data_process::msg_recv_finish()
{
    http_req_head_para & req_head_para = _base_process->get_req_head_para();
    LOG_DEBUG("url_path:%s", req_head_para._url_path.c_str());
    map<string, string>::iterator it;
    for (it = req_head_para._url_para_list.begin(); it != req_head_para._url_para_list.end(); it++)
        LOG_DEBUG("query:%s %s", it->first.c_str(), it->second.c_str());
    _recv_buf.clear();

    _body.append("I have recved, I am Server\n");
}

string * skhttp_req_data_process::get_send_head()
{
    string * str = new string;
    http_req_head_para & req_head = _base_process->get_req_head_para();

    char proc_name[SIZE_LEN_256] = {'\0'};
    get_proc_name(proc_name, sizeof(proc_name));

    req_head._headers.insert(make_pair("Date", SecToHttpTime(time(NULL))));
    req_head._headers.insert(make_pair("Server", proc_name));
    req_head._headers.insert(make_pair("Connection", "keep-alive"));
    req_head._content_length = _body.length();
    _base_process->gen_send_head(str);

    LOG_DEBUG("%s", str->c_str());
    return str;
}

size_t skhttp_req_data_process::process_recv_body(const char *buf, size_t len, int& result)
{
    result = 1;

    _recv_buf.append(buf, len);

    LOG_DEBUG("recv_buf: %s", recv_buf.c_str());
    return len;
}

base_net_obj * skhttp_req_data_process::gen_net_obj(http_req_msg * req_msg)
{
    url_info info; 
    parse_url(req_msg->url, _url_info);

    vector<string> vIp;
    parse_domain(info.domain, vIp);

    int index = rand() % vIP;
    out_connect<http_req_process> * connect = new out_connect<http_res_process>(vIP[index], info.port);
    http_req_process * req_process = new http_req_process(connect);
    skhttp_req_data_process * sk_process = new sah_data_process(req_process);
    req_process->set_process(sk_process);
    connect->set_process(req_process);

    sk_process->set_url_info(info);
    sk_process->set_req_msg(req_msg);

    return connect;
}

