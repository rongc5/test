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
#include "history_wsingle_dict.h"

#include "hquotation_search_index.h"


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
        auto & rquoation_dict_index = p_data->_rquotation_index->current()->id_quotation;
        auto it = rquoation_dict_index.find(id); 
        if (it != rquoation_dict_index.end())
        {    
            end = it->second.back()->end;
        }
    }

    if (!end)
    {
        std::string key;

        auto &  _hquoation_dict = p_data->_hquotation_index->current()->id_quotation;
        auto ii = _hquoation_dict.find(id);
        if (ii != _hquoation_dict.end())
        {
            if (ii->second.size())
            {
                end = ii->second.back()->end;
            }
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
        sst.dratio = (sst.in + sst.out)? sst.diff*1.0/(sst.in + sst.out):0; 

        tmp_single->push_back(sst);
        flag = true;
    }

    if (flag)
    {
        std::shared_ptr<single_vec> st = get_rsingle(tmp_single);
        p_data->_hsingle_dict->update_real_single(*p_data->get_trade_date(), _id, st);
        p_data->_hwsingle_dict->update_real_wsingle(*p_data->get_trade_date(), _id, st);
    }

over:
    throw CMyCommonException("msg_recv_finish");
}

std::shared_ptr<single_vec> rsingle_data_process::get_rsingle(std::shared_ptr<single_vec> & tmp_single)
{
    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current();
    std::shared_ptr<single_vec> single(new single_vec);
    uint32_t i = 0;

    for (i = 0; i < strategy->real_single_scale.size(); i++)
    {
        int index = get_single_index(_id, i);
        if (index < 0 || index > (int)tmp_single->size())
        {
            single_t tmp_st;
            single->push_back(tmp_st);
        }
        else
        {
            single->push_back(tmp_single->at(index));
        }
    }

    return single;
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
    //req_head._headers.insert(std::make_pair("Accept", "*/*"));

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
    std::shared_ptr<base_net_obj> connect = get_base_net();

    if (connect)
    {   
        std::shared_ptr<destroy_msg>  net_obj(new destroy_msg);
        net_obj->id = _id;
        net_obj->_msg_op = NORMAL_MSG_DESTROY_ST;

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

