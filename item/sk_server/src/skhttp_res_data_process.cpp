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
#include "real_quotation_dict.h"
#include "finance_dict.h"



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

void skhttp_res_data_process::query_quotation(std::string &id, Value & root, Document::AllocatorType & allocator)
{
    Value key(kStringType);
    Value value(kStringType);

    proc_data* p_data = proc_data::instance();
    auto ii = p_data->_rquoation_dict->current()->_id_dict.find(id);
    if (ii != p_data->_rquoation_dict->current()->_id_dict.end())
    {
        for (auto ft: *(ii->second.current()))
        {
            if (!strncmp(ft.first.c_str(), "id", strlen("id")))
                continue;

            key.SetString(ft.first.c_str(), allocator); 
            value.SetString(ft.second.c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }
    }
}

void skhttp_res_data_process::query_finance(std::string &id, Value & root, Document::AllocatorType & allocator)
{
    Value key(kStringType);
    Value value(kStringType);

    proc_data* p_data = proc_data::instance();
    auto ii = p_data->_finance_dict->current()->_id_dict.find(id);
    if (ii != p_data->_finance_dict->current()->_id_dict.end())
    {
        for (auto ft:ii->second)
        {
            if (!strncmp(ft.first.c_str(), "id", strlen("id")))
                continue;

            if (!strncmp(ft.first.c_str(), "time_str", strlen("time_str")))
                continue;

            key.SetString(ft.first.c_str(), allocator); 
            value.SetString(ft.second.c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }
    }
}

int skhttp_res_data_process::do_query_id(std::map<std::string, std::string> & url_para_map, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();

    {
        auto ii = url_para_map.find("id");
        if (ii == url_para_map.end())
        {
            return HTPP_RES_ERR;
        }
    }
    
    if (!p_data)
    {
        return HTPP_RES_ERR;
    }
    
    Value key(kStringType);
    Value value(kStringType);
    

    {
        key.SetString("id", allocator);
        value.SetString(url_para_map["id"].c_str(), allocator);

        root.AddMember(key, value, allocator);
    }

    {
        Value child(kObjectType);
        query_finance(url_para_map["id"], child, allocator);

        key.SetString("finance", allocator);
        root.AddMember(key, child, allocator);
    }

    {
        Value child(kObjectType);
        query_quotation(url_para_map["id"], child, allocator);

        key.SetString("quotation", allocator);
        root.AddMember(key, child, allocator);
    }

    return HTPP_RES_OK;
}

int skhttp_res_data_process::do_select(std::map<std::string, std::string> & url_para_map, Value & root, Document::AllocatorType & allocator)
{
    return HTPP_RES_OK;
}

void skhttp_res_data_process::msg_recv_finish()
{
    http_req_head_para & req_head_para = _base_process->get_req_head_para();
    LOG_DEBUG("url_path:%s", req_head_para._url_path.c_str());
    
    std::map<std::string, std::string> url_para_map;
    parse_url_para(req_head_para._url_path, url_para_map);

    Document document;
    Document::AllocatorType& allocator = document.GetAllocator(); 
    Value root(kObjectType); 
    Value child(kObjectType);

    int recode;
    if (!strncmp(req_head_para._url_path.c_str(), "/queryid", strlen("/queryid"))){
        recode = do_query_id(url_para_map, child, allocator);
    }else if (!strncmp(req_head_para._url_path.c_str(), "/select", strlen("/select"))){
        recode = do_select(url_para_map, child, allocator);
    } else {
        recode = HTPP_REQ_PATH_ERR;
    }
    
    Value key(kStringType);    
    Value value(kStringType); 

    key.SetString("recode", allocator);
    root.AddMember(key, recode, allocator);

    key.SetString("data", allocator);

    root.AddMember(key, child, allocator);

    StringBuffer buffer;    
    Writer<StringBuffer> writer(buffer);    
    root.Accept(writer);
    //_recv_buf.clear();

    LOG_DEBUG("response:%s", buffer.GetString());
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
    res_head._headers.insert(std::make_pair("Connection", "keep-alive"));
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
}

