#include "rquotation_data_process.h"
#include "http_base_process.h"
#include "http_base_data_process.h"
#include "log_helper.h"
#include "out_connect.h"
#include "common_obj_container.h"
#include "base_net_thread.h"

#include "base_def.h"
#include "http_req_process.h"
#include "proc_data.h"
#include "common_domain.h"
#include "sk_conf.h"
#include "real_quotation_dict.h"
#include "id_dict.h"

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
    //FILE_WRITE("123", _recv_buf.c_str());
    
    std::vector<std::string> strVec;
    SplitString(_recv_buf.c_str(), "=", &strVec, SPLIT_MODE_ONE);
    if (strVec.size() >= 2)
    {
        std::string value = trim(strVec[1].c_str(), ";\n");
        SplitString(value.c_str(), "~", &strVec, SPLIT_MODE_ALL);
    }

    _recv_buf.clear();
    proc_data * p_data = proc_data::instance();
    if (strVec.size() <= 45)
    {
        return;
    }

    auto it = p_data->_rquoation_dict->current()->_id_dict.find(_id);
    if (it == p_data->_rquoation_dict->current()->_id_dict.end())
    {
        LOG_WARNING("id: %s is not in _quotation_dict", _id.c_str());
        return;
    }

    std::map<std::string, std::string> * qt = it->second.idle();
    
    qt->insert(std::make_pair("name", strVec[1]));
    qt->insert(std::make_pair("start", strVec[5]));
    qt->insert(std::make_pair("end", strVec[3]));
    qt->insert(std::make_pair("high", strVec[33]));
    qt->insert(std::make_pair("low",  strVec[34]));
    qt->insert(std::make_pair("last_closed", strVec[4]));
    qt->insert(std::make_pair("vol",strVec[36]));
    qt->insert(std::make_pair("sell_vol", strVec[8]));
    qt->insert(std::make_pair("buy_vol", strVec[7]));
    qt->insert(std::make_pair("swing", strVec[43]));
    qt->insert(std::make_pair("change_rate", strVec[38]));
    qt->insert(std::make_pair("range_percent", strVec[32]));
    qt->insert(std::make_pair("total_price", strVec[37]));

    if (atof((*qt)["start"].c_str()) <= 1)
    {
        p_data->_block_set.idle()->insert(_id);
    }


    it->second.idle_2_current();

    throw CMyCommonException("msg_recv_finish");
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

void rquotation_data_process::set_url_info(url_info & u_info)
{
    _url_info = u_info;
}

void rquotation_data_process::set_id(std::string id)
{
    _id = id;
}

void rquotation_data_process::gen_net_obj(std::string id, common_obj_container * net_container, std::map<std::string, std::string> & headers)
{
    if (!net_container || id.empty())
        return;

    std::string url = "http://web.sqt.gtimg.cn/q=";
    url.append(id);

    std::string refer = "http://gu.qq.com/";
    refer.append(id);
    refer.append("/gp");

    url_info u_info;
    parse_url(url, u_info);

    std::vector<std::string> VP;
    std::vector<std::string> *vIp = net_container->get_domain()->get_domain(u_info.domain);
    if (!vIp)
    {
        parse_domain(u_info.domain, VP);
        vIp = &VP;
        if (!VP.empty())
        {
            net_container->get_domain()->add_domain(u_info.domain, VP);
        }
    }

    if (!vIp || vIp->empty())
    {
        return;
    }

    srand((uint32_t)&id);
    int index = rand() % vIp->size();
    u_info.ip = (*vIp)[index];

    std::shared_ptr<out_connect<http_req_process> >  
        connect(new out_connect<http_req_process>(u_info.ip, u_info.port));
    http_req_process * req_process = new http_req_process(connect);
    rquotation_data_process * sk_process = new rquotation_data_process(req_process);
    req_process->set_process(sk_process);
    connect->set_process(req_process);

    sk_process->set_url_info(u_info);
    sk_process->set_id(id);

    http_req_head_para & req_head = req_process->get_req_head_para();

    req_head._headers.insert(headers.begin(), headers.end());

    req_head._method = "GET";
    req_head._url_path = u_info.full_path;
    req_head._version = "HTTP/1.1";

    req_head._headers.insert(std::make_pair("Host", u_info.domain));
    req_head._headers.insert(std::make_pair("Accept", "*/*"));

    req_head._headers["Referer"] = refer;
    //req_head._headers["Connection"] = "close";

    connect->connect();
    connect->set_net_container(net_container);

    proc_data* p_data = proc_data::instance();
    if (p_data && p_data->_conf && p_data->_conf->req_http_timeout)
    {
        std::shared_ptr<timer_msg>  t_msg(new timer_msg);
        t_msg->_time_length = p_data->_conf->req_http_timeout;
        t_msg->_timer_type = TIMER_TYPE_HTTP_REQ; 
        t_msg->_obj_id = connect->get_id()._id;
        sk_process->add_timer(t_msg);
        LOG_NOTICE("add_timer TIMER_TYPE_HTTP_REQ");
    }

    return ;
}

void rquotation_data_process::handle_timeout(timer_msg & t_msg)
{
    if (t_msg._timer_type == TIMER_TYPE_HTTP_REQ)
    {
        LOG_NOTICE("timeout TIMER_TYPE_HTTP_REQ");

        throw CMyCommonException("TIMER_TYPE_HTTP_REQ");
    }
}

void rquotation_data_process::destroy()
{
    if (!_is_ok)
    {
        std::shared_ptr<base_net_obj> connect = get_base_net();
        if (connect)
        {
            common_obj_container * net_container = connect->get_net_container();
            net_container->get_domain()->delete_domain(_url_info.domain);
        }

        proc_data* p_data = proc_data::instance();
        id_dict * id_dic = p_data->_id_dict->current(); 
        if (id_dic && id_dic->_id_vec.size() && id_dic->_id_vec[id_dic->_id_vec.size() - 1] == _id)
        {
            p_data->_block_set.idle_2_current();
            LOG_DEBUG("_block_set idle_2_current");
        }
    }
}

