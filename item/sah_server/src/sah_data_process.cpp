#include "sah_data_process.h"
#include "http_base_process.h"
#include "http_base_data_process.h"
#include "log_helper.h"
#include "common_def.h"
#include "base_connect.h"
#include "base_net_container.h"
#include "base_net_thread.h"


sah_data_process::sah_data_process(http_base_process * _p_process):http_base_data_process(_p_process)
{

}

string * sah_data_process::get_send_body(int &result)
{
    result = 1;

    string * body = new string(_body);

    _body.clear();

    return body;
}

void sah_data_process::header_recv_finish()
{
    base_net_container * net_container = get_base_connect()->get_net_container();
}

void sah_data_process::msg_recv_finish()
{
    http_req_head_para & req_head_para = _base_process->get_req_head_para();
    LOG_DEBUG("url_path:%s", req_head_para._url_path.c_str());
    map<string, string>::iterator it;
    for (it = req_head_para._url_para_list.begin(); it != req_head_para._url_para_list.end(); it++)
        LOG_DEBUG("query:%s %s", it->first.c_str(), it->second.c_str());
    _recv_buf.clear();

    _body.append("I have recved, I am Server\n");
}

string * sah_data_process::get_send_head()
{
    string * str = new string;
    http_res_head_para & res_head = _base_process->get_res_head_para();
    
    char proc_name[SIZE_LEN_256] = {'\0'};
    get_proc_name(proc_name, sizeof(proc_name));

    res_head._headers.insert(make_pair("Date", SecToHttpTime(time(NULL))));
    res_head._headers.insert(make_pair("Server", proc_name));
    res_head._headers.insert(make_pair("Connection", "keep-alive"));
    res_head._content_length = _body.length();
    _base_process->gen_send_head(str);

    LOG_DEBUG("%s", str->c_str());
    return str;
}

size_t sah_data_process::process_recv_body(const char *buf, size_t len, int& result)
{
    result = 1;

    _recv_buf.append(buf, len);

    return len;
}


