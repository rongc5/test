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
#include "strategy_conf.h"
#include "id_dict.h"
#include "history_single_dict.h"

#include "history_quotation_dict.h"


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
    //http_res_head_para & res_head_para = _base_process->get_res_head_para();
    //std::string * str = new std::string;
    //res_head_para.to_head_str(str);
    //LOG_DEBUG("header: %s", str->c_str());

    //std::shared_ptr<std::string> rc(str);
    _is_ok = true;
}

int rsingle_data_process::get_single_index(const std::string &id, uint32_t index)
{
    int arr[] = {100, 200, 300, 400, 500, 800, 1000, 1500, 2000};
    int i = -1;

    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current();
    float end = 0;

    {
        auto it = p_data->_rquoation_dict_index.current()->find(id); 
        if (it != p_data->_rquoation_dict_index.current()->end())
        {    
            end = it->second->end;
        }
    }

    if (!end)
    {
        std::string key;
        auto ii = p_data->_hquoation_dict->current()->_id_date_dict.find(id);
        if (ii != p_data->_hquoation_dict->current()->_id_date_dict.end())
        {
            const std::string & date = *(ii->second.rbegin());
            history_quotation_dict::creat_key(date, id, key);

        }

        auto it = p_data->_hquoation_dict->current()->_id_dict.find(key);
        if (it != p_data->_hquoation_dict->current()->_id_dict.end())
        {
            end = it->second->end;
        }
    }

    if (!end)
        return i;

    
    for (uint32_t k = 0; k < sizeof(arr)/sizeof(arr[0]); k++)
    {
        if (end * arr[k] * 100 >= strategy->real_single_scale[index])
        {
            i = k;
            break;
        }
    }

    return i; 
}

void rsingle_data_process::msg_recv_finish()
{
    LOG_DEBUG("recv_buf: %s", _recv_buf.c_str());
    //FILE_WRITE("123", _recv_buf.c_str());
    proc_data * p_data = proc_data::instance();

    std::shared_ptr<single_vec> tmp_single(new single_vec);

    int ttmp;
    std::vector<std::string> ssVec;
    uint32_t i = 0;
    bool flag = false;
    
    std::vector<std::string> strVec;
    SplitString(_recv_buf.c_str(), "=", &strVec, SPLIT_MODE_ONE);
    if (strVec.size() < 2)
    {
        goto over;
    }

    _recv_buf.clear();


    //不是标准json
    
    SplitString(strVec[1].c_str(), ",", &ssVec, SPLIT_MODE_ALL);

    for (i = 3; i < ssVec.size() & i + 7 <= ssVec.size(); i += 7)
    {
        single_t sst;
        sst.in = atoi(ssVec[i + 4].c_str());
        sst.out = atoi(ssVec[i + 5].c_str());
        sst.diff = sst.in - sst.out;
        tmp_single->push_back(sst);
        flag = true;
    }

    if (flag)
    {
        p_data->_rsingle_real_dict[_id] = tmp_single;
    }

over:
    throw CMyCommonException("msg_recv_finish");
}

int rsingle_data_process::get_single_diff2(std::deque<std::shared_ptr<single_vec> > & st, uint32_t index)
{
    if (st.size() <= 0 || index >= st.back()->size())
        return -1;

    if (st.size() == 1)
    {
        return 0;
    }

    int diff2 = st.back()->at(index).diff - st.front()->at(index).diff;

    return diff2;
}

void rsingle_data_process::single_index_reset()
{
    proc_data* p_data = proc_data::instance();
    {
        std::vector<std::multimap<int, std::string> > t_map;
        p_data->_rsingle_diff_index.idle()->swap(t_map);
    }

    {
        std::vector<std::multimap<int, std::string> > t_map;
        p_data->_rsingle_diff2_index.idle()->swap(t_map);
    }


    {
        std::unordered_map<std::string, std::deque<std::shared_ptr<single_vec> >,str_hasher> tmp;
        p_data->_rsingle_dict_index.idle()->swap(tmp);
    }

    {
        std::vector<std::map<std::string, std::multimap<int, std::string> > > tmp;
        p_data->_hsingle_sum_diff_index.idle()->swap(tmp);
    }
}

void rsingle_data_process::update_all_index()
{
    single_index_reset();
    
    update_real_index();
    update_sum_index();

    single_idle_current();
}

bool rsingle_data_process::get_sum_diff(std::string & id, std::string & date, single_vec & st)
{
    proc_data* p_data = proc_data::instance();

    bool flag = false;
    auto it = p_data->_hsingle_dict->current()->_date_index.find(p_data->_trade_date);
    if (it != p_data->_hsingle_dict->current()->_date_index.end())
        flag =  true;

    std::string key;
    history_single_dict::creat_key(date, id, key);
    auto tt = p_data->_hsingle_dict->current()->_date_sum_dict.find(key);
    if (tt == p_data->_hsingle_dict->current()->_date_sum_dict.end())
        return false;

    st.clear();
    for (uint32_t i = 0; i < tt->second->size(); i++)
    {   
        if (i >= st.size())
        {   
            single_t ss; 
            st.push_back(ss);
        }

        st.at(i) = tt->second->at(i);

        LOG_DEBUG("st: id:%s i:%d in:%d out:%d diff:%d", id.c_str(), i, st[i].in, st[i].out, st[i].diff);
    }

    if (!flag)
    {
        auto ii = p_data->_rsingle_dict_index.current()->find(id);

        if (ii != p_data->_rsingle_dict_index.current()->end() && ii->second.size())
        {
            for (uint32_t i = 0; i != ii->second.back()->size(); i++)
            {
                st[i].diff = st[i].diff + ii->second.back()->at(i).diff;
                LOG_DEBUG("st: id:%s i:%d in:%d out:%d diff:%d", id.c_str(), i, st[i].in, st[i].out, st[i].diff);
            } 
        }
    }
    
    return true;
}

void rsingle_data_process::update_sum_index()
{
    proc_data* p_data = proc_data::instance();
    bool flag = false;
    
    auto it = p_data->_hsingle_dict->current()->_date_index.find(p_data->_trade_date);
    if (it != p_data->_hsingle_dict->current()->_date_index.end())
        flag =  true;

    strategy_conf * strategy = p_data->_conf->_strategy->current();
    for (uint32_t i = 0; i < strategy->real_single_scale.size(); i++)
    {
        std::map<std::string, std::multimap<int, std::string> > t_map;

        if (i >= p_data->_hsingle_sum_diff_index.idle()->size()) 
        {
            p_data->_hsingle_sum_diff_index.idle()->push_back(t_map);
        }
    }

    std::string key;
    for (auto it = p_data->_hsingle_dict->current()->_date_index.begin(); 
            it != p_data->_hsingle_dict->current()->_date_index.end(); it++)
    {   
        const std::string  & date = *it;
        for (auto ii = p_data->_rsingle_dict_index.idle()->begin(); ii != p_data->_rsingle_dict_index.idle()->end(); ii++)
        {
            const std::string & id = ii->first;
            std::deque<std::shared_ptr<single_vec> > & st = ii->second;
            std::string key;
            history_single_dict::creat_key(date, id, key);

            auto tt = p_data->_hsingle_dict->current()->_date_sum_dict.find(key);
            if (tt == p_data->_hsingle_dict->current()->_date_sum_dict.end())
                continue;

            {
                single_vec hs;
                for (uint32_t i = 0; i < tt->second->size(); i++)
                {   
                    if (i >= hs.size())
                    {   
                        single_t st; 
                        hs.push_back(st);
                    }

                    hs.at(i) = tt->second->at(i);
                    LOG_DEBUG("date:%s id:%s i:%d in:%d out:%d diff:%d", date.c_str(), id.c_str(), i, hs[i].in, hs[i].out, hs[i].diff);
                }

                if (!flag)
                {
                    for (uint32_t i =0; i < st.back()->size() && hs.size(); i++) 
                    {
                        hs[i].diff = hs[i].diff + st.back()->at(i).diff;
                        LOG_DEBUG("date:%s id:%s i:%d in:%d out:%d diff:%d", date.c_str(), id.c_str(), i, hs[i].in, hs[i].out, hs[i].diff);
                    } 
                }

                for (uint32_t i = 0; i < hs.size(); i++)
                {
                    if (hs.at(i).diff > 0)
                    {
                        std::map<std::string, std::multimap<int, std::string> >  & u_map = (*(p_data->_hsingle_sum_diff_index.idle()))[i];

                        auto ii = u_map.find(date);
                        if (ii == u_map.end())
                        {
                            std::multimap<int, std::string> t_map;

                            t_map.insert(std::make_pair(hs.at(i).diff, id));
                            u_map.insert(std::make_pair(date, t_map));
                        }
                        else
                        {
                            ii->second.insert(std::make_pair(hs.at(i).diff, id));  
                        }
                    }
                }

            }
        }

    }

}

void rsingle_data_process::update_real_index()
{
    proc_data* p_data = proc_data::instance();
    std::deque<std::shared_ptr<single_vec> > st;
    strategy_conf * strategy = p_data->_conf->_strategy->current();
    uint32_t i = 0;
    int diff2 = 0;

    for (i = 0; i < strategy->real_single_scale.size(); i++)
    {
        std::multimap<int, std::string> t_map;
        if (i >= p_data->_rsingle_diff_index.idle()->size()) 
        {
            p_data->_rsingle_diff_index.idle()->push_back(t_map);
        }

        if (i >= p_data->_rsingle_diff2_index.idle()->size())
        {
            p_data->_rsingle_diff2_index.idle()->push_back(t_map);
        }
    }

    for (auto ii = p_data->_rsingle_real_dict.begin(); ii != p_data->_rsingle_real_dict.end(); ii++)
    {
        st.clear();
        std::shared_ptr<single_vec> single(new single_vec);

        auto it = p_data->_rsingle_dict_index.current()->find(ii->first);
        if (it != p_data->_rsingle_dict_index.current()->end())
        {
            st = it->second;
        }

        for (i = 0; i < strategy->real_single_scale.size(); i++)
        {
            int index = get_single_index(ii->first, i);
            if (index < 0 || index > (int)ii->second->size())
            {
                single_t tmp_st;
                single->push_back(tmp_st);
            }
            else
            {
                single->push_back(ii->second->at(index));
            }
        }

        if (st.empty() || st.back()->at(0) != single->at(0))
        {
            st.push_back(single);
        }

        if (p_data->_conf->_strategy->current()->real_single_deque_length && 
                st.size() > p_data->_conf->_strategy->current()->real_single_deque_length)
        {
            st.pop_front();
        }

        {
            p_data->_rsingle_dict_index.idle()->insert(std::make_pair(ii->first, st));
        }

        for (i = 0; i< single->size(); i++)
        {
            diff2 = get_single_diff2(st, i);
            if (diff2 > 0)
            {
                std::multimap<int, std::string> & t_map = 
                    (*(p_data->_rsingle_diff2_index.idle()))[i];

                t_map.insert(std::make_pair(diff2, ii->first));
            }

            if (single->at(i).diff > 0)
            {
                std::multimap<int, std::string> & t_map = 
                    (*(p_data->_rsingle_diff_index.idle()))[i];

                t_map.insert(std::make_pair(single->at(i).diff, ii->first));
            }
        }
    }
}

void rsingle_data_process::single_idle_current()
{
    proc_data* p_data = proc_data::instance();
    p_data->_rsingle_diff_index.idle_2_current();
    p_data->_rsingle_diff2_index.idle_2_current();
    p_data->_rsingle_dict_index.idle_2_current();
    p_data->_hsingle_sum_diff_index.idle_2_current();
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

    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current();
    std::string url = strategy->real_single_api;
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

    srand((unsigned long)vIp);
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

void rsingle_data_process::handle_timeout(std::shared_ptr<timer_msg> & t_msg)
{
    if (t_msg->_timer_type == TIMER_TYPE_HTTP_REQ)
    {
        LOG_DEBUG("timeout TIMER_TYPE_HTTP_REQ");

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

