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
#include "plate_dict.h"
#include "addr_dict.h"
#include "history_single_dict.h"



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

void skhttp_res_data_process::query_plate(std::string &id, Value & root, Document::AllocatorType & allocator)
{
    Value value(kStringType);
    proc_data* p_data = proc_data::instance();
    auto ii = p_data->_plate_dict->current()->_id_dict.find(id);
    if (ii != p_data->_plate_dict->current()->_id_dict.end())
    {
        for (auto ft: ii->second)
        {
            value.SetString(ft.c_str(), allocator); 
            root.PushBack(value, allocator);
        }
    }
}

int skhttp_res_data_process::get_single_index(std::string &id, uint32_t num)
{
    int arr[] = {100, 200, 300, 400, 500, 800, 1000, 1500, 2000};
    int index = -1;
    proc_data* p_data = proc_data::instance();
    auto it = p_data->_rquoation_dict->current()->_id_dict.find(id); 
    if (it != p_data->_rquoation_dict->current()->_id_dict.end())
    {
        if (has_key<std::string, std::string>(*(it->second.current()), "end"))
        {
            float end = atof((*(it->second.current()))["end"].c_str());
             
            for (uint32_t i = 0; i< sizeof(arr)/sizeof(arr[0]); i++)
            {
                if (end * arr[i] * 100 >= num)
                {
                    index = i;
                    break;
                }
            }
        }
    }

    return index;
}

void skhttp_res_data_process::query_single(std::string &id, Value & root, Document::AllocatorType & allocator)
{

    int index = 0;
    char t_buf[SIZE_LEN_64];
    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current();
    auto ii = p_data->_rsingle_dict->current()->_id_dict.find(id);

    if (ii != p_data->_rsingle_dict->current()->_id_dict.end())
    {
        for (uint32_t i = 0; i < strategy->real_single_scale.size(); i++)
        {
            index = get_single_index(id, strategy->real_single_scale[i]);
            if (index < 0)
                break;

            
            Value key(kStringType);
            Value child(kArrayType);

            snprintf(t_buf, sizeof(t_buf), "vol_%d", i);
            key.SetString(t_buf, allocator);

            for (auto ft: *(ii->second.current()))
            {
                if (index < (int)ft.size())
                {
                    child.PushBack(Value().SetInt(ft[index]), allocator);
                }
            }
            root.AddMember(key, child, allocator);
        }
    }
}

void skhttp_res_data_process::query_blocked(std::string &id, Value & root, Document::AllocatorType & allocator)
{
    Value key(kStringType);
    Value value(kStringType);

    key.SetString("block", allocator);
    proc_data* p_data = proc_data::instance();
    auto ii = p_data->_block_set.current()->find(id);
    if (ii != p_data->_block_set.current()->end())
    {
        root.AddMember(key, 1, allocator);
    }
    else
    {
        root.AddMember(key, 0, allocator);
    }
}

void skhttp_res_data_process::query_addr(std::string &id, Value & root, Document::AllocatorType & allocator)
{
    Value key(kStringType);
    Value value(kStringType);

    key.SetString("addr", allocator);
    proc_data* p_data = proc_data::instance();
    auto ii = p_data->_addr_dict->current()->_id_dict.find(id);
    if (ii != p_data->_addr_dict->current()->_id_dict.end())
    {
        std::string str;
        for (auto ft:ii->second)
        {
            str.append(ft);
        }
        value.SetString(str.c_str(), allocator);
        root.AddMember(key, value, allocator);
    }
    else
    {
        root.AddMember(key, value, allocator);
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

void skhttp_res_data_process::query_history_single(uint32_t last_day_num, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();
    char t_buf[SIZE_LEN_64];

    std::vector<std::string> date_vec;
    
    {
        auto ii = p_data->_hsid_date_index.current()->find(id);
        if (ii == p_data->_hsid_date_index.current()->end())
        {
            return;
        }
        
        uint32_t i = 0;
        for (auto iii = ii->second.rbegin(); iii != ii->second.rend() && i < last_day_num; i++, iii++)
        {
            date_vec.push_back(*iii);
        }
    }
    
    for (uint32_t i = 0; i< date_vec.size(); i++)
    {
        std::string key;    
        history_single_dict::creat_key(date_vec[i], id, key);

        auto ii = p_data->_hsingle_dict->current()->_date_dict.find(key);
        if (ii != p_data->_hsingle_dict->current()->_date_dict.end())
        {
            Value k(kStringType);
            Value child(kArrayType);

            std::string t_str;
            t_str.append("last_single");
            t_str.append("_");
            t_str.append(date_vec[i]);

            k.SetString(t_str.c_str(), allocator);

            for (uint32_t k = 0; k < ii->second.hs_vec.size(); k++)
            {
                child.PushBack(ii->second.hs_vec[k].single, allocator);
            }

            root.AddMember(k, child, allocator);
        }
    }
}

int skhttp_res_data_process::url_query_id(std::map<std::string, std::string> & url_para_map, Value & data_array, Document::AllocatorType & allocator)
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
    Value root(kObjectType);
    
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

    {
        Value child(kArrayType);
        query_plate(url_para_map["id"], child, allocator);

        key.SetString("plate", allocator);
        root.AddMember(key, child, allocator);
    }

    {
        Value child(kObjectType);
        query_single(url_para_map["id"], child, allocator);

        key.SetString("single", allocator);
        root.AddMember(key, child, allocator);
    }

    {
        query_blocked(url_para_map["id"], root, allocator);
    }

    {
        query_addr(url_para_map["id"], root, allocator);
    }
    
    if (has_key<std::string, std::string>(url_para_map, "last_day_num"))
    {
        query_history_single(atoi(url_para_map["last_day_num"].c_str()), url_para_map["id"], root, allocator);
    }

    data_array.PushBack(root, allocator);

    return HTPP_RES_OK;
}

void skhttp_res_data_process::do_check_end(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res)
{
    float end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();
    
    std::map<float, std::vector<std::string> >::iterator it_le, it_ge, it;
    it_le = p_data->_end_index.current()->end();
    it_ge = p_data->_end_index.current()->begin();

    if (has_key<std::string, string>(url_para_map, "end_le"))
    {
        flag = true;
        end = atof(url_para_map["end_le"].c_str());
        it_le = p_data->_end_index.current()->upper_bound(end);
    }

    if (has_key<std::string, string>(url_para_map, "end_ge"))
    {
        flag = true;
        end = atof(url_para_map["end_ge"].c_str());
        it_ge = p_data->_end_index.current()->lower_bound(end);
    }

    if (flag)
        return;

    for (it = it_ge; it < it_le; it++)
    {
        for (auto iii; iii != it.end(); iii++)
            res.insert(iii); 
    }
}

void skhttp_res_data_process::do_check_change_rate(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res)
{
    float end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();
    
    std::map<float, std::vector<std::string> >::iterator it_le, it_ge, it;
    it_le = p_data->_change_rate_index.current()->end();
    it_ge = p_data->_change_rate_index.current()->begin();

    if (has_key<std::string, string>(url_para_map, "change_rate_le"))
    {
        flag = true;
        end = atof(url_para_map["change_rate_le"].c_str());
        it_le = p_data->_change_rate_index.current()->upper_bound(end);
    }

    if (has_key<std::string, string>(url_para_map, "change_rate_ge"))
    {
        flag = true;
        end = atof(url_para_map["change_rate_ge"].c_str());
        it_ge = p_data->_change_rate_index.current()->lower_bound(end);
    }

    if (flag)
        return;

    for (it = it_ge; it < it_le; it++)
    {
        for (auto iii; iii != it.end(); iii++)
            res.insert(iii); 
    }
}

int skhttp_res_data_process::do_check_select(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res)
{
    if (has_key<std::string, string>)
}

int skhttp_res_data_process::url_select(std::map<std::string, std::string> & url_para_map, Value & root, Document::AllocatorType & allocator)
{
    return HTPP_RES_OK;
}

void skhttp_res_data_process::msg_recv_finish()
{
    http_req_head_para & req_head_para = _base_process->get_req_head_para();
    LOG_DEBUG("url_path:%s", req_head_para._url_path.c_str());
    
    std::map<std::string, std::string> url_para_map;

    Document document;
    Document::AllocatorType& allocator = document.GetAllocator(); 
    Value root(kObjectType); 
    Value data_array(kArrayType);

    int recode;
    if (!strncmp(req_head_para._url_path.c_str(), "/queryid", strlen("/queryid"))){
        parse_url_para(req_head_para._url_path, url_para_map);
        recode = do_query_id(url_para_map, data_array, allocator);
    }else if (!strncmp(req_head_para._url_path.c_str(), "/select", strlen("/select"))){
        do_parse_request(url_para_map);
        recode = do_select(url_para_map, data_array, allocator);
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

int skhttp_res_data_process::do_parse_request(std::map<std::string, std::string> & url_para_map)
{
   if (_recv_buf.empty())
       return;

    std::vector<std::string> items;
    SplitString(_recv_buf.c_str(), "\r\n", &items, SPLIT_MODE_ALL);

    for (uint32_t i = 0; i < items.size(); i++)
    {
        std::vector<std::string> sub_items;
        SplitString(items[i].c_str(), "=", &sub_items, SPLIT_MODE_ALL);

        if (sub_items.size() >= 2)
        {
            url_para_map[trim(sub_items[0].c_str())] = trim(sub_items[1].c_str());
        }
    }

    return 0;
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

