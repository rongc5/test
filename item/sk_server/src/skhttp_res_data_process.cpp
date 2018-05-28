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
#include "writer.h"
#include "stringbuffer.h"
#include "real_quotation_dict.h"
#include "finance_dict.h"

using namespace rapidjson;

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

void skhttp_res_data_process::query_quotation(std::string &id, StringBuffer & ss)
{
    auto ii = p_data->_rquoation_dict->current()->_id_dict.find(id);
    if (ii != p_data->_finance_dict->current()->_id_dict.end())
    {
        Writer<StringBuffer> ws(ss);
    }
}

void skhttp_res_data_process::query_finance(std::string &id, StringBuffer & ss)
{
    auto ii = p_data->_finance_dict->current()->_id_dict.find(id);
    if (ii != p_data->_finance_dict->current()->_id_dict.end())
    {
        Writer<StringBuffer> ws(ss);

        ws.StartObject();
        ws.Key("mgxj");
        ws.Double(ii->second.mgxj);

        ws.Key("mgsy");
        ws.Double(ii->second.mgsy);

        ws.Key("mgsygr");
        ws.Double(ii->second.mgsygr);

        ws.Key("mgxjgr");
        ws.Double(ii->second.mgxjgr);

        ws.Key("zysrgr");
        ws.Double(ii->second.zysrgr);

        ws.Key("yylrgr");
        ws.Double(ii->second.yylrgr);

        ws.Key("jlrgr");
        ws.Double(ii->second.jlrgr);

        ws.Key("pe");
        ws.Uint(ii->second.pe);

        ws.Key("pb");
        ws.Uint(ii->second.pb);

        ws.Key("value");
        ws.Uint(ii->second.value);

        ws.Key("cir_value");
        ws.Uint(ii->second.cir_value);
        ws.EndObject();
    }
}

void skhttp_res_data_process::do_query_id(url_info & u_info, std::string & res)
{
    proc_data* p_data = proc_data::instance();

    std::map<std::string, std::string> url_para_map;
    parse_url_para(u_info.query, url_para_map);

    {
        auto ii = url_para_map.find("id");
        if (ii == url_para_map.end())
        {
            do_req_err();
            return;
        }
    }
    
    if (!p_data)
    {
        do_req_err();
        return;
    }
    

    StringBuffer s;
    Writer<StringBuffer> writer(s);

    writer.StartObject();

    {
        StringBuffer ss;
        query_finance(url_para_map["id"], ss);
        if (!ss.GetString().empty())
        {
            writer.Key("finance");
            writer.String(ss.GetString());
        }
    }

    writer.EndObject();
}

void skhttp_res_data_process::do_select(url_info & u_info, std::string & res)
{

}

void skhttp_res_data_process::do_req_err(std::string & res)
{

}

void skhttp_res_data_process::msg_recv_finish()
{
    http_req_head_para & req_head_para = _base_process->get_req_head_para();
    LOG_DEBUG("url_path:%s", req_head_para._url_path.c_str());
    url_info u_info;
    
    parse_url(req_head_para._url_path, u_info);

    if (!strncmp(u_info.path.c_str(), "queryid", strlen("queryid"))){
        do_query_id(u_info, _body);
    }else if (!strncmp(u_info.path.c_str(), "select", strlen("select"))){
        do_select(u_info, _body);
    }else {
        do_req_err(_body);    
    }


    _recv_buf.clear();

    _body.append("I have recved, I am Server\n");
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

