#include "skhttp_res_data_process.h"
#include "http_base_process.h"
#include "http_base_data_process.h"
#include "log_helper.h"
#include "base_connect.h"
#include "common_obj_container.h"
#include "base_net_thread.h"
#include "common_util.h"
#include "base_def.h"
#include "http_res_process.h"
#include "proc_data.h"

skhttp_res_data_process::skhttp_res_data_process(http_base_process * _p_process):http_base_data_process(_p_process)
{

}

std::string * skhttp_res_data_process::get_send_body(int &result)
{
    result = 1;

    std::string * body = new std::string(_body);

    _body.clear();

    return body;
}


void skhttp_res_data_process::msg_recv_finish()
{
    proc_data* p_data = proc_data::instance();
    http_req_head_para & req_head_para = _base_process->get_req_head_para();

    std::shared_ptr<base_net_obj> net_obj = get_base_net();
    net_addr & peer_addr = net_obj->get_peer_addr();

    LOG_NOTICE("peer ip[%s] peer port[%d] url_path[%s]", peer_addr.ip.c_str(), peer_addr.port, req_head_para._url_path.c_str());

    std::vector<std::string> tmp_vec;
    SplitString(req_head_para._url_path.c_str(), '?', &tmp_vec, SPLIT_MODE_ALL);
    if (tmp_vec.size() < 2) 
    {   
        tmp_vec.push_back(req_head_para._url_path);
    }  

    _current_hander = p_data->get_url_handler(tmp_vec[0]);
    
    if (_current_hander)
        _current_hander->perform(&req_head_para, &_recv_buf, &_base_process->get_res_head_para(), &_body);

    LOG_NOTICE("response:%s", _body.c_str());
}

std::string * skhttp_res_data_process::get_send_head()
{
    std::string * str = new std::string;
    http_res_head_para & res_head = _base_process->get_res_head_para();

    res_head.to_head_str(str);

    LOG_DEBUG("%s", str->c_str());

    return str;
}


size_t skhttp_res_data_process::process_recv_body(const char *buf, size_t len, int& result)
{
    result = 1;

    _recv_buf.append(buf, len);

    return len;
}

void skhttp_res_data_process::gen_listen_obj(int fd, common_obj_container * net_container)
{
    std::shared_ptr<base_connect<http_res_process> > connect(new base_connect<http_res_process>(fd));
    http_res_process * res_process = new http_res_process(connect);
    skhttp_res_data_process * sa_process = new skhttp_res_data_process(res_process);
    res_process->set_process(sa_process);
    connect->set_process(res_process);

    connect->set_net_container(net_container);
}

