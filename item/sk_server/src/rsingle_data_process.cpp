#include "rsingle_data_process.h"
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
#include "real_single_dict.h"
#include "strategy_conf.h"


rsingle_data_process::rsingle_data_process(http_base_process * _p_process):http_base_data_process(_p_process)
{
    _is_ok = false;
}

std::string * rsingle_data_process::get_send_body(int &result)
{
    result = 1;

    return NULL;
}


url_info & rsingle_data_process::get_url_info()
{
    return _url_info;
}

void rsingle_data_process::header_recv_finish()
{
    //base_net_container * net_container = get_base_net()->get_net_container();
    http_res_head_para & res_head_para = _base_process->get_res_head_para();
    std::string * str = new std::string;
    res_head_para.to_head_str(str);
    LOG_DEBUG("header: %s", str->c_str());

    std::shared_ptr<std::string> rc(str);
    _is_ok = true;
}

void rsingle_data_process::msg_recv_finish()
{
    LOG_DEBUG("recv_buf: %s", _recv_buf.c_str());
    //FILE_WRITE("123", _recv_buf.c_str());
    
    std::vector<std::string> strVec;
    SplitString(_recv_buf.c_str(), "=", &strVec, SPLIT_MODE_ONE);
    if (strVec.size() < 2)
    {
        return;
    }

    _recv_buf.clear();
    proc_data * p_data = proc_data::instance();

    auto it = p_data->_rsingle_dict->current()->_id_dict.find(_id);
    if (it == p_data->_rsingle_dict->current()->_id_dict.end())
    {
        return;
    }

    std::deque<std::vector<int> > * st = it->second.idle();
    *st = *it->second.current();
    std::vector<int> single;
    //不是标准json
    
    int ttmp;
    std::vector<std::string> ssVec;
    SplitString(strVec[1].c_str(), ",", &ssVec, SPLIT_MODE_ALL);
    

    for (uint32_t i = 3; i < ssVec.size() & i + 7 <= ssVec.size(); i += 7)
    {
        ttmp = atoi(ssVec[i + 4].c_str()) - atoi(ssVec[i + 5].c_str());
        single.push_back(ttmp);
    }
    if (single.empty())
    {
        return;
    }


    if (st->empty() || (*(st->rbegin()))[0] != single[0])
    {
        st->push_back(single);
    }

    if (p_data->_conf->_strategy->current()->real_single_deque_length && 
            st->size() > p_data->_conf->_strategy->current()->real_single_deque_length)
    {
        st->pop_front();
    }

    it->second.idle_2_current();

    throw CMyCommonException("msg_recv_finish");
}

std::string * rsingle_data_process::get_send_head()
{
    http_req_head_para & req_head = _base_process->get_req_head_para();


    std::string * str = new std::string;
    req_head.to_head_str(str);
    LOG_DEBUG("%s", str->c_str());

    return str;
}

size_t rsingle_data_process::process_recv_body(const char *buf, size_t len, int& result)
{
    result = 1;

    _recv_buf.append(buf, len);

    //LOG_DEBUG("recv_buf: %s", _recv_buf.c_str());
    return len;
}

void rsingle_data_process::set_url_info(url_info & u_info)
{
    _url_info = u_info;
}

void rsingle_data_process::set_id(std::string id)
{
    _id = id;
}

void rsingle_data_process::gen_net_obj(std::string id, common_obj_container * net_container, std::map<std::string, std::string> & headers)
{
    if (!net_container || id.empty())
        return;

    std::string url = "http://stock.gtimg.cn/data/index.php?appn=dadan&action=summary&c=";
    url.append(id);

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
    rsingle_data_process * sk_process = new rsingle_data_process(req_process);
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

    //req_head._headers["Connection"] = "close";

    connect->connect();
    connect->set_net_container(net_container);

    proc_data* p_data = proc_data::instance();
    if (p_data && p_data->_conf && p_data->_conf->_strategy->current()->req_http_timeout)
    {
        std::shared_ptr<timer_msg>  t_msg(new timer_msg);
        t_msg->_time_length = p_data->_conf->_strategy->current()->req_http_timeout;
        t_msg->_timer_type = TIMER_TYPE_HTTP_REQ; 
        t_msg->_obj_id = connect->get_id()._id;
        sk_process->add_timer(t_msg);
        LOG_NOTICE("add_timer TIMER_TYPE_HTTP_REQ");
    }

    return ;
}

void rsingle_data_process::handle_timeout(timer_msg & t_msg)
{
    if (t_msg._timer_type == TIMER_TYPE_HTTP_REQ)
    {
        LOG_NOTICE("timeout TIMER_TYPE_HTTP_REQ");

        throw CMyCommonException("TIMER_TYPE_HTTP_REQ");
    }
}

void rsingle_data_process::destroy()
{
    if (!_is_ok)
    {
        std::shared_ptr<base_net_obj> connect = get_base_net();
        if (connect)
        {
            common_obj_container * net_container = connect->get_net_container();
            net_container->get_domain()->delete_domain(_url_info.domain);
        }
    }
}
