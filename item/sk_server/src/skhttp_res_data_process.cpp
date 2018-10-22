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
#include "finance_dict.h"
#include "plate_dict.h"
#include "addr_dict.h"
#include "history_single_dict.h"
#include "history_quotation_dict.h"
#include "base_net_obj.h"
#include "id_dict.h"

#include "sk_util.h"
#include "rsingle_data_process.h"
#include "rquotation_data_process.h"

#include "uhandler_select.h"


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

void skhttp_res_data_process::header_recv_finish()
{

}




void skhttp_res_data_process::msg_recv_finish()
{
    http_req_head_para & req_head_para = _base_process->get_req_head_para();

    std::shared_ptr<base_net_obj> net_obj = get_base_net();
    net_addr & peer_addr = net_obj->get_peer_addr();

    LOG_NOTICE("peer ip[%s] peer port[%d] url_path[%s]", peer_addr.ip.c_str(), peer_addr.port, req_head_para._url_path.c_str());

    int recode;
    if (!strncmp(req_head_para._url_path.c_str(), "/queryid", strlen("/queryid"))){
        parse_url_para(req_head_para._url_path, url_para_map);
        recode = url_query_id(url_para_map, data_array, allocator);
    }else if (!strncmp(req_head_para._url_path.c_str(), "/select", strlen("/select"))){
        do_parse_request(url_para_map);
        recode = url_select(url_para_map, data_array, allocator);
    } else {
        recode = HTPP_REQ_PATH_ERR;
    }

    Value key(kStringType);    
    Value value(kStringType); 

    key.SetString("recode", allocator);
    root.AddMember(key, recode, allocator);

    key.SetString("data", allocator);

    root.AddMember(key, data_array, allocator);

    StringBuffer buffer;    
    Writer<StringBuffer> writer(buffer);    
    root.Accept(writer);
    //_recv_buf.clear();

    LOG_NOTICE("response:%s", buffer.GetString());
    _body.append(buffer.GetString());
}

std::string * skhttp_res_data_process::get_send_head()
{
    std::string * str = new std::string;
    http_res_head_para & res_head = _base_process->get_res_head_para();

    char proc_name[SIZE_LEN_256] = {'\0'};
    proc_data* p_data = proc_data::instance();

    res_head._headers.insert(std::make_pair("Date", SecToHttpTime(time(NULL))));
    res_head._headers.insert(std::make_pair("Server", p_data->proc_name));
    //res_head._headers.insert(std::make_pair("Connection", "keep-alive"));
    res_head._headers.insert(std::make_pair("Connection", "close"));
    snprintf(proc_name, sizeof(proc_name), "%d", _body.length());
    res_head._headers.insert(std::make_pair("Content-Length", proc_name));
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

    sa_process->reg_handler("/select?", std::make_shared<uhandler_select>());
    sa_process->reg_handler("/queryid?", std::make_shared<uhandler_select>());
}

void reg_handler(std::string & url, std::shared_ptr<url_handler> & handler)
{
    _uhandler_map[url] = handler;
}

