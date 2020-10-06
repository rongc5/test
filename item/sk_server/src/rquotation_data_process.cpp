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
#include "id_dict.h"
#include "history_quotation_dict.h"
#include "strategy_conf.h"
#include "history_wquotation_dict.h"

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
    //http_res_head_para & res_head_para = _base_process->get_res_head_para();
    //std::string * str = new std::string;
    //res_head_para.to_head_str(str);
    //LOG_DEBUG("header: %s", str->c_str());

    //std::shared_ptr<std::string> rc(str);
    _is_ok = true;
}

void rquotation_data_process::msg_recv_finish()
{
    LOG_DEBUG("recv_buf: %s", _recv_buf.c_str());
    //FILE_WRITE("123", _recv_buf.c_str());
    char t_buf[SIZE_LEN_512];
    std::shared_ptr<quotation_original> qt(new quotation_original);
    float down_pointer = 0;
    float up_pointer = 0;
    float avg_end = 0;
    float end_avg_end = 0;
    
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
        goto over;
    }
    
    snprintf(qt->name, sizeof(qt->name), "%s", strVec[1].c_str());
    qt->start = atof(strVec[5].c_str());
    qt->end = atof(strVec[3].c_str());
    qt->high = atof(strVec[33].c_str());
    qt->low = atof(strVec[34].c_str());
    qt->last_closed = atof(strVec[4].c_str());
    qt->vol = atoi(strVec[36].c_str());
    qt->sell_vol= atoi(strVec[8].c_str());
    qt->buy_vol = atoi(strVec[7].c_str());
    qt->swing = atof(strVec[43].c_str());
    qt->change_rate = atof(strVec[38].c_str());
    qt->range_percent = atof(strVec[32].c_str());
    qt->total_price = atof(strVec[37].c_str());
    qt->quantity_relative_ratio = atof(strVec[49].c_str());
    
    

    if (qt->start <= 1)
    {
        p_data->_block_set->idle()->insert(_id);
    }
    else
    {
        p_data->_hquoation_dict->update_real_quotation(p_data->_trade_date, _id, qt);
        p_data->_hwquoation_dict->update_real_wquotation(p_data->_trade_date, _id, qt);
    }

over:
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

    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current();
    std::string url = strategy->real_quotation_api;
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

    srand((unsigned long)vIp);
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
    //req_head._headers.insert(std::make_pair("Accept", "*/*"));

    //req_head._headers["Referer"] = refer;
    //req_head._headers["Connection"] = "close";

    connect->connect();
    connect->set_net_container(net_container);

    if (p_data && p_data->_conf && p_data->_conf->_strategy->current()->req_http_timeout)
    {
        std::shared_ptr<timer_msg>  t_msg(new timer_msg);
        t_msg->_time_length = p_data->_conf->_strategy->current()->req_http_timeout;
        t_msg->_timer_type = TIMER_TYPE_HTTP_REQ; 
        t_msg->_obj_id = connect->get_id()._id;
        sk_process->add_timer(t_msg);
        LOG_DEBUG("add_timer TIMER_TYPE_HTTP_REQ");
    }

    return ;
}

void rquotation_data_process::handle_timeout(std::shared_ptr<timer_msg> & t_msg)
{
    if (t_msg->_timer_type == TIMER_TYPE_HTTP_REQ)
    {
        LOG_DEBUG("timeout TIMER_TYPE_HTTP_REQ:_timer_id %u _timer_type:%u", t_msg->_timer_id, t_msg->_timer_type);

        throw CMyCommonException("TIMER_TYPE_HTTP_REQ");
    }
}


void rquotation_data_process::destroy()
{
    LOG_DEBUG("id:%s destroy", _id.c_str());

    std::shared_ptr<base_net_obj> connect = get_base_net();

    if (connect)
    {   
        std::shared_ptr<destroy_msg>  net_obj(new destroy_msg);
        net_obj->id = _id;
        net_obj->_msg_op = NORMAL_MSG_DESTROY_QT;

        common_obj_container * net_container = connect->get_net_container();
        ObjId id; 
        id._id = OBJ_ID_THREAD;
        id._thread_index = net_container->get_thread_index();
        std::shared_ptr<normal_msg> ng = std::static_pointer_cast<normal_msg>(net_obj);
        base_net_thread::put_obj_msg(id, ng);

        if (!_is_ok) 
        {   
            net_container->get_domain()->delete_domain(_url_info.domain);
        }   
    }
}

