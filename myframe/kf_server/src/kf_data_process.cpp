#include "kf_data_process.h"
#include "http_base_process.h"
#include "http_base_data_process.h"
#include "log_helper.h"
#include "common_def.h"
#include "base_connect.h"
#include "base_net_container.h"
#include "base_net_thread.h"



kf_data_process::kf_data_process(http_base_process * _p_process):http_base_data_process(_p_process)
{

}

string * kf_data_process::get_send_body(int &result)
{
    result = 1;

    string * body = new string(_body);

    _body.clear();

    return body;
}

void kf_data_process::header_recv_finish()
{
    http_req_head_para & req_head_para = _base_process->get_req_head_para();
    
    string * tmp_str = req_head_para.get_header("Upgrade");
    if (tmp_str && strcasestr(tmp_str->c_str(), "websocket")) {
        get_base_connect()->remove_net_container();
        get_base_connect()->_msg_op = PASS_NEW_MSG;
        get_base_connect()->set_real_net(true);
        base_net_thread * net_thread = get_base_connect()->get_net_container()->get_net_thread();
        ObjId id;
        id._thread_index = net_thread->get_thread_index();
        net_thread->put_msg(id, get_base_connect());
        LOG_DEBUG("this is websocket");
    } else {
        _body.append("just a test");
    }
}

void kf_data_process::msg_recv_finish()
{

}

string * kf_data_process::get_send_head()
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

size_t kf_data_process::process_recv_body(const char *buf, size_t len, int& result)
{
    result = 1;

    return len;
}


