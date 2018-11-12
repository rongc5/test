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
    std::shared_ptr<quotation_t> qt(new quotation_t);
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
    
    snprintf(qt->id, sizeof(qt->id), "%s", _id.c_str());
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
    
    

    if (qt->start <= 1)
    {
        p_data->_block_set.idle()->insert(_id);
    }

    p_data->_rquoation_real_dict[_id] = qt;

over:
    throw CMyCommonException("msg_recv_finish");
}

void rquotation_data_process::quotation_index_reset()
{
    proc_data* p_data = proc_data::instance();
    {   
        std::multimap<float, std::string> t_map;
        p_data->_end_index.idle()->swap(t_map);
    }   

    {   
        std::multimap<float, std::string> t_map;
        p_data->_change_rate_index.idle()->swap(t_map);
    }   

    {   
        std::multimap<float, std::string> t_map;
        p_data->_range_percent_index.idle()->swap(t_map);
    }   

    {   
        std::multimap<float, std::string> t_map;
        p_data->_down_pointer_index.idle()->swap(t_map);
    }   

    {   
        std::multimap<float, std::string> t_map;
        p_data->_up_pointer_index.idle()->swap(t_map);
    }   

    {   
        std::multimap<float, std::string> t_map;
        p_data->_end_avg_end_index.idle()->swap(t_map);
    }   

    {   
        std::multimap<float, std::string> t_map;
        p_data->_end_avg_end5_index.idle()->swap(t_map);
    }   

    {   
        std::multimap<float, std::string> t_map;
        p_data->_end_avg_end10_index.idle()->swap(t_map);
    }   

    {   
        std::multimap<float, std::string> t_map;
        p_data->_end_avg_end20_index.idle()->swap(t_map);
    }   

    {   
        std::multimap<float, std::string> t_map;
        p_data->_end_avg_end30_index.idle()->swap(t_map);
    }   

    {   
        std::unordered_map<std::string, std::shared_ptr<quotation_t>,str_hasher> tmp;
        p_data->_rquoation_dict_index.idle()->swap(tmp);
    }  

    {   
        std::unordered_map<std::string, std::shared_ptr<technical_t>,str_hasher> tmp;
        p_data->_rtechnical_dict_index.idle()->swap(tmp);
    }  

    {   
        std::map<std::string, std::multimap<float, std::string> > tmp;
        p_data->_hq_sum_change_rate_index.idle()->swap(tmp);
    }  

    {   
        std::map<std::string, std::multimap<float, std::string> > tmp;
        p_data->_hq_sum_range_percent_index.idle()->swap(tmp);
    }  

    {
        std::multimap<float, std::string> tmp;
        p_data->_end_end5_index.idle()->swap(tmp);
    }

    {
        std::multimap<float, std::string> tmp;
        p_data->_end_end10_index.idle()->swap(tmp);
    }

    {
        std::multimap<float, std::string> tmp;
        p_data->_end_end20_index.idle()->swap(tmp);
    }

    {
        std::multimap<float, std::string> tmp;
        p_data->_end_end30_index.idle()->swap(tmp);
    }
}

bool rquotation_data_process::get_sum_quotation(std::string & id, std::string & date, quotation_t & qt)
{
    proc_data* p_data = proc_data::instance();

    bool flag = false;
    auto it = p_data->_hquoation_dict->current()->_date_index.find(p_data->_trade_date);
    if (it != p_data->_hquoation_dict->current()->_date_index.end())
        flag =  true;

    std::string key;
    history_quotation_dict::creat_key(date, id, key);

    auto tt = p_data->_hquoation_dict->current()->_id_sum_dict.find(key);
    if (tt == p_data->_hquoation_dict->current()->_id_sum_dict.end())
        return false;

    qt.range_percent = tt->second->range_percent; 
    if (!flag)
    {
        auto ii = p_data->_rquoation_dict_index.current()->find(id);
        if (ii != p_data->_rquoation_dict_index.current()->end())
            qt.range_percent += ii->second->range_percent;
    }

    return true;
}

void rquotation_data_process::update_sum_index()
{
    proc_data* p_data = proc_data::instance();
    bool flag = false;

    auto it = p_data->_hquoation_dict->current()->_date_index.find(p_data->_trade_date);
    if (it != p_data->_hquoation_dict->current()->_date_index.end())
        flag =  true;
    
    for (auto it = p_data->_hquoation_dict->current()->_date_index.begin(); 
            it != p_data->_hquoation_dict->current()->_date_index.end(); it++)
    {
        const std::string  & date = *it;

        for (auto ii = p_data->_rquoation_real_dict.begin(); ii != p_data->_rquoation_real_dict.end(); ii++)
        {
            const std::string & id = ii->first;
            std::string key;
            history_quotation_dict::creat_key(date, id, key);

            auto tt = p_data->_hquoation_dict->current()->_id_sum_dict.find(key);
            if (tt == p_data->_hquoation_dict->current()->_id_sum_dict.end())
                continue;

            {   
                float range_percent = 0;
                if (flag) 
                {
                    range_percent = tt->second->range_percent; 
                }
                else
                {
                    range_percent = tt->second->range_percent + ii->second->range_percent;
                }

                std::map<std::string, std::multimap<float, std::string> >  & u_map = *(p_data->_hq_sum_range_percent_index.idle());
                auto ii = u_map.find(date);
                if (ii == u_map.end())
                {   
                    std::multimap<float, std::string> t_map;

                    t_map.insert(std::make_pair(range_percent, id));
                    u_map.insert(std::make_pair(date, t_map));
                }       
                else
                {   
                    ii->second.insert(std::make_pair(range_percent, id));
                }
            } 

            {   
                float change_rate = 0;
                if (flag) 
                {
                    change_rate = tt->second->change_rate; 
                }
                else
                {
                    change_rate = tt->second->change_rate + ii->second->change_rate;
                }

                std::map<std::string, std::multimap<float, std::string> >  & u_map = *(p_data->_hq_sum_change_rate_index.idle());
                auto ii = u_map.find(date);
                if (ii == u_map.end())
                {   
                    std::multimap<float, std::string> t_map;

                    t_map.insert(std::make_pair(change_rate, id));
                    u_map.insert(std::make_pair(date, t_map));
                }       
                else
                {   
                    ii->second.insert(std::make_pair(change_rate, id));
                }
            }
    
        }
    }
}

void rquotation_data_process::update_all_index()
{
    quotation_index_reset();

    update_real_index();
    update_sum_index();

    idle_current();
}

void rquotation_data_process::update_id_technical(std::shared_ptr<quotation_t> qt)
{
    char t_buf[SIZE_LEN_512];
    t_buf[0] = '\0';
    if (!qt)
        return;
    
    proc_data* p_data = proc_data::instance();
    bool flag = false;

    auto it = p_data->_hquoation_dict->current()->_date_index.find(p_data->_trade_date);
    if (it != p_data->_hquoation_dict->current()->_date_index.end())
        flag =  true;

    std::shared_ptr<technical_t> tt(new technical_t);
    
    if ((qt->end > qt->low) && (qt->end != qt->start))
    {
        tt->down_pointer = (qt->end - qt->low)/
            (qt->end - qt->start);

        if (tt->down_pointer < 0)
            tt->down_pointer *= -1;

        snprintf(t_buf, sizeof(t_buf), "%.2f", tt->down_pointer);
        tt->down_pointer = atof(t_buf);
    }

    if ((qt->end < qt->high) && (qt->end != qt->start))
    {
        tt->up_pointer = (qt->high - qt->end)/
            (qt->end - qt->start);

        if (tt->up_pointer < 0)
            tt->up_pointer *= -1;

        snprintf(t_buf, sizeof(t_buf), "%.2f", tt->up_pointer);
        tt->up_pointer = atof(t_buf);
    }

    if (qt->vol)
    {
        tt->avg_end = (qt->total_price * 10000)/(qt->vol * 100);
        snprintf(t_buf, sizeof(t_buf), "%.2f", tt->avg_end);
        tt->avg_end = atof(t_buf);
    }

    if (tt->avg_end)
    {
        float end_avg_end = qt->end/tt->avg_end;

        snprintf(t_buf, sizeof(t_buf), "%.2f", end_avg_end);
        end_avg_end = atof(t_buf);

        p_data->_end_avg_end_index.idle()->insert(std::make_pair(end_avg_end, qt->id));
    }

    std::string date;
    std::string key;
    if (flag)
    {
        p_data->_hquoation_dict->current()->get_last_date(5, date);
        if (!date.empty())
        {
            history_quotation_dict::creat_key(date, qt->id, key);
            auto ii = p_data->_hquoation_dict->current()->_id_sum_dict.find(key);
            if (ii != p_data->_hquoation_dict->current()->_id_sum_dict.end())
            {
                tt->avg_end_5 = ii->second->avg_end/5;
                tt->end_5 = ii->second->end/5;
            }
        }

        p_data->_hquoation_dict->current()->get_last_date(10, date);
        if (!date.empty())
        {
            history_quotation_dict::creat_key(date, qt->id, key);
            auto ii = p_data->_hquoation_dict->current()->_id_sum_dict.find(key);
            if (ii != p_data->_hquoation_dict->current()->_id_sum_dict.end())
            {
                tt->avg_end_10 = ii->second->avg_end/10;
                tt->end_10 = ii->second->end/10;
            }
        }

        p_data->_hquoation_dict->current()->get_last_date(20, date);
        if (!date.empty())
        {
            history_quotation_dict::creat_key(date, qt->id, key);
            auto ii = p_data->_hquoation_dict->current()->_id_sum_dict.find(key);
            if (ii != p_data->_hquoation_dict->current()->_id_sum_dict.end())
            {
                tt->avg_end_20 = ii->second->avg_end/20;
                tt->end_20 = ii->second->end/20;
            }
        }

        p_data->_hquoation_dict->current()->get_last_date(30, date);
        if (!date.empty())
        {
            history_quotation_dict::creat_key(date, qt->id, key);
            auto ii = p_data->_hquoation_dict->current()->_id_sum_dict.find(key);
            if (ii != p_data->_hquoation_dict->current()->_id_sum_dict.end())
            {
                tt->avg_end_30 = ii->second->avg_end/30;
                tt->end_30 = ii->second->end/30;
            }
        }
    }
    else
    {
        p_data->_hquoation_dict->current()->get_last_date(4, date);
        if (!date.empty())
        {
            history_quotation_dict::creat_key(date, qt->id, key);
            auto ii = p_data->_hquoation_dict->current()->_id_sum_dict.find(key);
            if (ii != p_data->_hquoation_dict->current()->_id_sum_dict.end())
            {
                tt->avg_end_5 = (ii->second->avg_end + tt->avg_end)/5;
                tt->end_5 = (ii->second->end + qt->end)/5;
            }
        }

        p_data->_hquoation_dict->current()->get_last_date(9, date);
        if (!date.empty())
        {
            history_quotation_dict::creat_key(date, qt->id, key);
            auto ii = p_data->_hquoation_dict->current()->_id_sum_dict.find(key);
            if (ii != p_data->_hquoation_dict->current()->_id_sum_dict.end())
            {
                tt->avg_end_10 = (ii->second->avg_end + tt->avg_end)/10;
                tt->end_10 = (ii->second->end + qt->end)/10;
            }
        }

        p_data->_hquoation_dict->current()->get_last_date(19, date);
        if (!date.empty())
        {
            history_quotation_dict::creat_key(date, qt->id, key);
            auto ii = p_data->_hquoation_dict->current()->_id_sum_dict.find(key);
            if (ii != p_data->_hquoation_dict->current()->_id_sum_dict.end())
            {
                tt->avg_end_20 = (ii->second->avg_end + tt->avg_end)/20;
                tt->end_20 = (ii->second->end + qt->end)/20;
            }
        }

        p_data->_hquoation_dict->current()->get_last_date(29, date);
        if (!date.empty())
        {
            history_quotation_dict::creat_key(date, qt->id, key);
            auto ii = p_data->_hquoation_dict->current()->_id_sum_dict.find(key);
            if (ii != p_data->_hquoation_dict->current()->_id_sum_dict.end())
            {
                tt->avg_end_30 = (ii->second->avg_end + tt->avg_end)/30;
                tt->end_30 = (ii->second->end + qt->end)/30;
            }
        }
    }

    p_data->_rtechnical_dict_index.idle()->insert(std::make_pair(qt->id, tt));

    if (tt->avg_end_5 && qt->end)
        p_data->_end_avg_end5_index.idle()->insert(std::make_pair(qt->end/tt->avg_end_5, qt->id));

    if (tt->avg_end_10 && qt->end)
        p_data->_end_avg_end10_index.idle()->insert(std::make_pair(qt->end/tt->avg_end_10, qt->id));

    if (tt->avg_end_20 && qt->end)
        p_data->_end_avg_end20_index.idle()->insert(std::make_pair(qt->end/tt->avg_end_20, qt->id));

    if (tt->avg_end_30 && qt->end)
        p_data->_end_avg_end30_index.idle()->insert(std::make_pair(qt->end/tt->avg_end_30, qt->id));

    if (tt->end_5 && qt->end)
        p_data->_end_end5_index.idle()->insert(std::make_pair(qt->end/tt->end_5, qt->id));

    if (tt->end_10 && qt->end)
        p_data->_end_end10_index.idle()->insert(std::make_pair(qt->end/tt->end_10, qt->id));

    if (tt->end_20 && qt->end)
        p_data->_end_end20_index.idle()->insert(std::make_pair(qt->end/tt->end_20, qt->id));

    if (tt->end_30 && qt->end)
        p_data->_end_end30_index.idle()->insert(std::make_pair(qt->end/tt->end_30, qt->id));
}

void rquotation_data_process::update_real_index()
{
    proc_data* p_data = proc_data::instance();

    for (auto ii = p_data->_rquoation_real_dict.begin(); ii != p_data->_rquoation_real_dict.end(); ii++)
    {
        {
            p_data->_end_index.idle()->insert(std::make_pair(ii->second->end, ii->first));
        }

        {
            p_data->_change_rate_index.idle()->insert(std::make_pair(ii->second->change_rate, ii->first));
        }

        {
            p_data->_range_percent_index.idle()->insert(std::make_pair(ii->second->range_percent, ii->first));
        }

        {
            p_data->_rquoation_dict_index.idle()->insert(std::make_pair(ii->first, ii->second));
        }

        update_id_technical(ii->second);
    }
}

void rquotation_data_process::idle_current()
{
    proc_data* p_data = proc_data::instance();

    p_data->_end_index.idle_2_current();
    p_data->_change_rate_index.idle_2_current();
    p_data->_range_percent_index.idle_2_current();
    p_data->_end_avg_end_index.idle_2_current();
    p_data->_rquoation_dict_index.idle_2_current();
    p_data->_hq_sum_change_rate_index.idle_2_current();
    p_data->_hq_sum_range_percent_index.idle_2_current();

    p_data->_rtechnical_dict_index.idle_2_current();

    p_data->_end_avg_end5_index.idle_2_current();
    p_data->_end_avg_end10_index.idle_2_current();
    p_data->_end_avg_end20_index.idle_2_current();
    p_data->_end_avg_end30_index.idle_2_current();
    p_data->_end_end5_index.idle_2_current();
    p_data->_end_end10_index.idle_2_current();
    p_data->_end_end20_index.idle_2_current();
    p_data->_end_end30_index.idle_2_current();
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
    req_head._headers.insert(std::make_pair("Accept", "*/*"));

    req_head._headers["Referer"] = refer;
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

