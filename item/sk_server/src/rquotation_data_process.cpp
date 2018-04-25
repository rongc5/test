#include "rquotation_data_process.h"
#include "http_base_process.h"
#include "http_base_data_process.h"
#include "log_helper.h"
#include "out_connect.h"
#include "base_net_container.h"
#include "base_net_thread.h"

#include "base_def.h"
#include "http_req_process.h"

#include "proc_data.h"

rquotation_data_process::rquotation_data_process(http_base_process * _p_process):http_base_data_process(_p_process)
{
    _is_ok = false;
}

std::string * rquotation_data_process::get_send_body(int &result)
{
    result = 1;

    return NULL;
}


url_info & rquotation_data_process::get_url_info()
{
    return _url_info;
}

void rquotation_data_process::header_recv_finish()
{
    //base_net_container * net_container = get_base_net()->get_net_container();
    http_res_head_para & res_head_para = _base_process->get_res_head_para();
    std::string * str = new std::string;
    res_head_para.to_head_str(str);
    LOG_DEBUG("header: %s", str->c_str());

    std::shared_ptr<std::string> rc(str);
    _is_ok = true;
}

void rquotation_data_process::msg_recv_finish()
{
    LOG_DEBUG("recv_buf: %s", _recv_buf.c_str());
    
    std::vector<std::string> strVec;
    SplitString(_recv_buf.c_str(), "=", &strVec, SPLIT_MODE_ONE);
    if (strVec.size() >= 2)
    {
        std::string value = trim(strVec[1].c_str(), ";\n");
        SplitString(value.c_str(), "~", &strVec, SPLIT_MODE_ALL);
    }

    _recv_buf.clear();
}

std::string * rquotation_data_process::get_send_head()
{
    http_req_head_para & req_head = _base_process->get_req_head_para();


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

void rquotation_data_process::gen_net_obj(std::string id, common_obj_container * net_container, td::map<std::string, std::string> & headers)
{
    if (!net_container || id.empty())
        return;

    string url = "http://web.sqt.gtimg.cn/q=";
    url.append(id);

    string refer = "http://gu.qq.com/";
    refer.append(id);
    refer.append("/gp");

    url_info u_info;
    parse_url(url, _url_info);

    std::string<std::string> VP;
    std::vector<std::string> *vIp = net_container->get_domain()->get_domain(_url_info.domain);
    if (!vIp)
    {
        parse_domain(_url_info.domain, VP);
        vIp = Vp;
        if (!VP.empty())
        {
            net_container->get_domain()->add_domain(_url_info.domain, VP);
        }
    }

    if (!vIp || vIp.empty())
    {
        return;
    }

    srand((uint32_t)&id);
    int index = rand() % vIp->size();
    _url_info.ip = vIp[index];

    std::shared_ptr<out_connect<http_req_process> >  
        connect(new out_connect<http_req_process>(_url_info.ip, _url_info.port));
    http_req_process * req_process = new http_req_process(connect);
    rquotation_data_process * sk_process = new rquotation_data_process(req_process);
    req_process->set_process(sk_process);
    connect->set_process(req_process);

    http_req_head_para & req_head = req_process->get_req_head_para();

    req_head._method = "GET";
    req_head._url_path = _url_info.full_path;
    req_head._version = "HTTP/1.1";

    req_head._headers.insert(std::make_pair("Host", _url_info.domain));
    req_head._headers.insert(std::make_pair("Accept", "*/*"));

    req_head._headers = headers;
    req_head._headers["Referer"] = refer;

    connect->connect();
    connect->set_net_container(net_container);

    if (req_msg.time_out)
    {
        timer_msg  t_msg;
        t_msg._time_length = req_msg.time_out;
        t_msg._timer_type = TIMER_TYPE_HTTP_REQ; 
        sk_process->add_timer(t_msg);
    }

    return ;
}

void rquotation_data_process::handle_timeout(timer_msg & t_msg)
{
    if (t_msg._timer_type == TIMER_TYPE_HTTP_REQ)
    {
        if (!_is_ok)
        {
            throw CMyCommonException("TIMER_TYPE_HTTP_REQ");
        }
    }

    return true;
}

