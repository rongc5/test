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
#include "finance_dict.h"
#include "plate_dict.h"
#include "addr_dict.h"
#include "history_single_dict.h"
#include "history_quotation_dict.h"
#include "base_net_obj.h"
#include "id_dict.h"

#include <algorithm>
#include "sk_util.h"
#include "rsingle_data_process.h"



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
    auto ii = p_data->_rquoation_dict_index.current()->find(id);
    if (ii != p_data->_rquoation_dict_index.current()->end())
    {
        {
            key.SetString("name", allocator); 
            value.SetString(ii->second->name, allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("start", allocator); 
            value.SetString(float_2_str(ii->second->start).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("end", allocator); 
            value.SetString(float_2_str(ii->second->end).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("high", allocator); 
            value.SetString(float_2_str(ii->second->high).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("low", allocator); 
            value.SetString(float_2_str(ii->second->low).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("last_closed", allocator); 
            value.SetString(float_2_str(ii->second->last_closed).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("change_rate", allocator); 
            value.SetString(float_2_str(ii->second->change_rate).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("range_percent", allocator); 
            value.SetString(float_2_str(ii->second->range_percent).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("down_pointer", allocator); 
            value.SetString(float_2_str(ii->second->down_pointer).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("up_pointer", allocator); 
            value.SetString(float_2_str(ii->second->up_pointer).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("avage_price", allocator); 
            value.SetString(float_2_str(ii->second->avg_price).c_str(), allocator); 

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
            value.SetString(ft->c_str(), allocator); 
            root.PushBack(value, allocator);
        }
    }
}

void skhttp_res_data_process::query_single(std::string &id, Value & root, Document::AllocatorType & allocator)
{
    char t_buf[SIZE_LEN_64];
    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current();
    auto ii = p_data->_rsingle_dict_index.current()->find(id);

    if (ii != p_data->_rsingle_dict_index.current()->end())
    {
        for (uint32_t i = 0; i < strategy->real_single_scale.size(); i++)
        {
            Value key(kStringType);
            Value child(kArrayType);

            snprintf(t_buf, sizeof(t_buf), "vol_%d", i);
            key.SetString(t_buf, allocator);

            for (auto ft: ii->second)
            {
                if (i < ft->size())
                {
                    child.PushBack(Value().SetInt(ft->at(i).diff), allocator);
                }
            }
            root.AddMember(key, child, allocator);
        }
    }
}

void skhttp_res_data_process::query_single_in(std::string &id, Value & root, Document::AllocatorType & allocator)
{
    char t_buf[SIZE_LEN_64];
    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current();
    auto ii = p_data->_rsingle_dict_index.current()->find(id);

    if (ii != p_data->_rsingle_dict_index.current()->end())
    {
        for (uint32_t i = 0; i < strategy->real_single_scale.size(); i++)
        {
            Value key(kStringType);
            Value child(kArrayType);

            snprintf(t_buf, sizeof(t_buf), "vol_%d", i);
            key.SetString(t_buf, allocator);

            for (auto ft: ii->second)
            {
                if (i < ft->size())
                {
                    child.PushBack(Value().SetInt(ft->at(i).in), allocator);
                }
            }
            root.AddMember(key, child, allocator);
        }
    }
}

void skhttp_res_data_process::query_single_out(std::string &id, Value & root, Document::AllocatorType & allocator)
{
    char t_buf[SIZE_LEN_64];
    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current();
    auto ii = p_data->_rsingle_dict_index.current()->find(id);

    if (ii != p_data->_rsingle_dict_index.current()->end())
    {
        for (uint32_t i = 0; i < strategy->real_single_scale.size(); i++)
        {
            Value key(kStringType);
            Value child(kArrayType);

            snprintf(t_buf, sizeof(t_buf), "vol_%d", i);
            key.SetString(t_buf, allocator);

            for (auto ft: ii->second)
            {
                if (i < ft->size())
                {
                    child.PushBack(Value().SetInt(ft->at(i).out), allocator);
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
            str.append(ft->c_str());
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
    auto ii = p_data->_finance_dict_index.current()->find(id);
    if (ii != p_data->_finance_dict_index.current()->end())
    {
        {
            key.SetString("pe", allocator); 
            value.SetString(int_2_str(ii->second->pe).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("pb", allocator); 
            value.SetString(int_2_str(ii->second->pb).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("cir_value", allocator); 
            value.SetString(int_2_str(ii->second->cir_value).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("value", allocator); 
            value.SetString(int_2_str(ii->second->value).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("mgsy", allocator); 
            value.SetString(float_2_str(ii->second->mgsy).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("mgxj", allocator); 
            value.SetString(float_2_str(ii->second->mgxj).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("mgsygr", allocator); 
            value.SetString(float_2_str(ii->second->mgsygr).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("mgxjgr", allocator); 
            value.SetString(float_2_str(ii->second->mgxjgr).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("zysrgr", allocator); 
            value.SetString(float_2_str(ii->second->zysrgr).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("yylrgr", allocator); 
            value.SetString(float_2_str(ii->second->yylrgr).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("jlrgr", allocator); 
            value.SetString(float_2_str(ii->second->jlrgr).c_str(), allocator); 

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
        auto ii = p_data->_hsingle_dict->current()->_id_date_dict.find(id);
        if (ii == p_data->_hsingle_dict->current()->_id_date_dict.end())
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
            t_str.append("single");
            t_str.append("_");
            t_str.append(date_vec[i]);

            k.SetString(t_str.c_str(), allocator);

            for (uint32_t k = 0; k < ii->second->size(); k++)
            {
                child.PushBack(ii->second->at(k).diff, allocator);
            }

            root.AddMember(k, child, allocator);
        }
    }
}

void skhttp_res_data_process::query_sum_single(uint32_t last_day_num, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();

    std::string date;

    auto ii = p_data->_hsingle_dict->current()->_id_date_dict.find(id);
    if (ii == p_data->_hsingle_dict->current()->_id_date_dict.end())
    {
        return;
    }

    uint32_t i = 0;
    for (auto iii = ii->second.rbegin(); iii != ii->second.rend() && i < last_day_num; i++, iii++)
    {
        date = *iii;
    }

    query_sum_single(date, id, root, allocator);
}

void skhttp_res_data_process::query_history_single_in(uint32_t last_day_num, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();
    char t_buf[SIZE_LEN_64];

    std::vector<std::string> date_vec;

    {
        auto ii = p_data->_hsingle_dict->current()->_id_date_dict.find(id);
        if (ii == p_data->_hsingle_dict->current()->_id_date_dict.end())
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
            t_str.append("single_in");
            t_str.append("_");
            t_str.append(date_vec[i]);

            k.SetString(t_str.c_str(), allocator);

            for (uint32_t k = 0; k < ii->second->size(); k++)
            {
                child.PushBack(ii->second->at(k).in, allocator);
            }

            root.AddMember(k, child, allocator);
        }
    }
}

void skhttp_res_data_process::query_history_single_out(uint32_t last_day_num, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();
    char t_buf[SIZE_LEN_64];

    std::vector<std::string> date_vec;

    {
        auto ii = p_data->_hsingle_dict->current()->_id_date_dict.find(id);
        if (ii == p_data->_hsingle_dict->current()->_id_date_dict.end())
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
            t_str.append("single_out");
            t_str.append("_");
            t_str.append(date_vec[i]);

            k.SetString(t_str.c_str(), allocator);

            for (uint32_t k = 0; k < ii->second->size(); k++)
            {
                child.PushBack(ii->second->at(k).out, allocator);
            }

            root.AddMember(k, child, allocator);
        }
    }
}

void skhttp_res_data_process::query_history_single(std::string & history_date, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();

    std::string key;    
    history_single_dict::creat_key(history_date, id, key);

    auto ii = p_data->_hsingle_dict->current()->_date_dict.find(key);
    if (ii != p_data->_hsingle_dict->current()->_date_dict.end())
    {
        Value k(kStringType);
        Value child(kArrayType);

        std::string t_str;
        t_str.append("single");
        t_str.append("_");
        t_str.append(history_date);

        k.SetString(t_str.c_str(), allocator);

        for (uint32_t k = 0; k < ii->second->size(); k++)
        {
            child.PushBack(ii->second->at(k).diff, allocator);
        }

        root.AddMember(k, child, allocator);
    }
}

void skhttp_res_data_process::query_sum_single(std::string & history_date, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();

    single_vec st;
    bool flag = rsingle_data_process::get_sum_diff(id, history_date, st);
    if (flag)
    {
        Value k(kStringType);
        Value child(kArrayType);

        std::string t_str;
        t_str.append("sum_single");
        t_str.append("_");
        t_str.append(history_date);

        k.SetString(t_str.c_str(), allocator);

        for (uint32_t k = 0; k < st.size(); k++)
        {
            child.PushBack(st.at(k).diff, allocator);
        }

        root.AddMember(k, child, allocator);
    }
}

void skhttp_res_data_process::query_history_single_in(std::string & history_date, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();

    std::string key;    
    history_single_dict::creat_key(history_date, id, key);

    auto ii = p_data->_hsingle_dict->current()->_date_dict.find(key);
    if (ii != p_data->_hsingle_dict->current()->_date_dict.end())
    {
        Value k(kStringType);
        Value child(kArrayType);

        std::string t_str;
        t_str.append("single_in");
        t_str.append("_");
        t_str.append(history_date);

        k.SetString(t_str.c_str(), allocator);

        for (uint32_t k = 0; k < ii->second->size(); k++)
        {
            child.PushBack(ii->second->at(k).in, allocator);
        }

        root.AddMember(k, child, allocator);
    }
}

void skhttp_res_data_process::query_history_single_out(std::string & history_date, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();

    std::string key;    
    history_single_dict::creat_key(history_date, id, key);

    auto ii = p_data->_hsingle_dict->current()->_date_dict.find(key);
    if (ii != p_data->_hsingle_dict->current()->_date_dict.end())
    {
        Value k(kStringType);
        Value child(kArrayType);

        std::string t_str;
        t_str.append("single_out");
        t_str.append("_");
        t_str.append(history_date);

        k.SetString(t_str.c_str(), allocator);

        for (uint32_t k = 0; k < ii->second->size(); k++)
        {
            child.PushBack(ii->second->at(k).out, allocator);
        }

        root.AddMember(k, child, allocator);
    }
}

void skhttp_res_data_process::query_history_quotation(uint32_t last_day_num, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();
    char t_buf[SIZE_LEN_64];

    std::vector<std::string> date_vec;

    {
        auto ii = p_data->_hquoation_dict->current()->_id_date_dict.find(id);
        if (ii == p_data->_hquoation_dict->current()->_id_date_dict.end())
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
        history_quotation_dict::creat_key(date_vec[i], id, key);

        auto ii = p_data->_hquoation_dict->current()->_id_dict.find(key);
        if (ii != p_data->_hquoation_dict->current()->_id_dict.end())
        {
            Value k(kStringType);
            Value v(kObjectType);

            Value key(kStringType);
            Value value(kStringType);

            std::string t_str;
            t_str.append("quotation");
            t_str.append("_");
            t_str.append(date_vec[i]);

            k.SetString(t_str.c_str(), allocator);

            {
                key.SetString("end", allocator); 
                value.SetString(float_2_str(ii->second->end).c_str(), allocator); 

                v.AddMember(key, value, allocator);
            }

            {
                key.SetString("low", allocator); 
                value.SetString(float_2_str(ii->second->low).c_str(), allocator); 

                v.AddMember(key, value, allocator);
            }

            {
                key.SetString("range_percent", allocator); 
                value.SetString(float_2_str(ii->second->range_percent).c_str(), allocator); 

                v.AddMember(key, value, allocator);
            }

            root.AddMember(k, v, allocator);
        }
    }
}

void skhttp_res_data_process::query_history_quotation(std::string & history_date, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();
    char t_buf[SIZE_LEN_64];


    std::string key;    
    history_quotation_dict::creat_key(history_date, id, key);

    auto ii = p_data->_hquoation_dict->current()->_id_dict.find(key);
    if (ii != p_data->_hquoation_dict->current()->_id_dict.end())
    {
        Value k(kStringType);
        Value v(kObjectType);

        Value key(kStringType);
        Value value(kStringType);

        std::string t_str;
        t_str.append("quotation");
        t_str.append("_");
        t_str.append(history_date);

        k.SetString(t_str.c_str(), allocator);

        {
            key.SetString("end", allocator); 
            value.SetString(float_2_str(ii->second->end).c_str(), allocator); 

            v.AddMember(key, value, allocator);
        }

        {
            key.SetString("low", allocator); 
            value.SetString(float_2_str(ii->second->low).c_str(), allocator); 

            v.AddMember(key, value, allocator);
        }

        {
            key.SetString("range_percent", allocator); 
            value.SetString(float_2_str(ii->second->range_percent).c_str(), allocator); 

            v.AddMember(key, value, allocator);
        }

        root.AddMember(k, v, allocator);
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

        get_standard_id(url_para_map["id"]);
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

    if (has_key<std::string, std::string>(url_para_map, "single_in"))
    {
        Value child(kObjectType);
        query_single_in(url_para_map["id"], child, allocator);

        key.SetString("single_in", allocator);
        root.AddMember(key, child, allocator);
    }

    if (has_key<std::string, std::string>(url_para_map, "single_out"))
    {
        Value child(kObjectType);
        query_single_out(url_para_map["id"], child, allocator);

        key.SetString("single_out", allocator);
        root.AddMember(key, child, allocator);
    }


    {
        query_blocked(url_para_map["id"], root, allocator);
    }

    {
        query_addr(url_para_map["id"], root, allocator);
    }

    if (has_key<std::string, std::string>(url_para_map, "history_num"))
    {
        query_history_single(atoi(url_para_map["history_num"].c_str()), url_para_map["id"], root, allocator);

        query_sum_single(atoi(url_para_map["history_num"].c_str()), url_para_map["id"], root, allocator);

        if (has_key<std::string, std::string>(url_para_map, "single_in"))
            query_history_single_in(atoi(url_para_map["history_num"].c_str()), url_para_map["id"], root, allocator);

        if (has_key<std::string, std::string>(url_para_map, "single_out"))
            query_history_single_out(atoi(url_para_map["history_num"].c_str()), url_para_map["id"], root, allocator);

        query_history_quotation(atoi(url_para_map["history_num"].c_str()), url_para_map["id"], root, allocator);
    }

    if (has_key<std::string, std::string>(url_para_map, "history_date"))
    {
        query_history_single(url_para_map["history_date"], url_para_map["id"], root, allocator);

        query_sum_single(url_para_map["history_date"], url_para_map["id"], root, allocator);

        if (has_key<std::string, std::string>(url_para_map, "single_in"))
            query_history_single_in(url_para_map["history_date"], url_para_map["id"], root, allocator);

        if (has_key<std::string, std::string>(url_para_map, "single_out"))
            query_history_single_out(url_para_map["history_date"], url_para_map["id"], root, allocator);

        query_history_quotation(url_para_map["history_date"], url_para_map["id"], root, allocator);
    }

    data_array.PushBack(root, allocator);

    return HTPP_RES_OK;
}

bool skhttp_res_data_process::do_check_end_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    float end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();

    std::multimap<float, std::string>::iterator it_le, it_ge, it;
    it_le = p_data->_end_index.current()->end();
    it_ge = p_data->_end_index.current()->begin();

    if (has_key<std::string, std::string>(url_para_map, "end_le"))
    {
        flag = true;
        end = atof(url_para_map["end_le"].c_str());
        it_le = p_data->_end_index.current()->upper_bound(end);

        if (SETS_OP_UNION == et)
            res = search;
    }

    for (it = it_ge; it != it_le && flag; ++it)
    {
        if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
            res.insert(it->second); 
    }

    return flag;
}

bool skhttp_res_data_process::do_check_end_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    float end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();

    std::multimap<float, std::string>::iterator it_le, it_ge, it;
    it_le = p_data->_end_index.current()->end();
    it_ge = p_data->_end_index.current()->begin();

    if (has_key<std::string, std::string>(url_para_map, "end_ge"))
    {
        flag = true;
        end = atof(url_para_map["end_ge"].c_str());
        it_ge = p_data->_end_index.current()->lower_bound(end);

        if (SETS_OP_UNION == et)
            res = search;
    }

    for (it = it_ge; it != it_le && flag; ++it)
    {
        if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
            res.insert(it->second); 
    }

    return flag;
}

bool skhttp_res_data_process::do_check_change_rate_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    float end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();

    std::multimap<float, std::string>::iterator it_le, it_ge, it;
    it_le = p_data->_change_rate_index.current()->end();
    it_ge = p_data->_change_rate_index.current()->begin();

    if (has_key<std::string, std::string>(url_para_map, "change_rate_le"))
    {
        flag = true;
        end = atof(url_para_map["change_rate_le"].c_str());
        it_le = p_data->_change_rate_index.current()->upper_bound(end);

        if (SETS_OP_UNION == et)
            res = search;
    }

    for (it = it_ge; it != it_le && flag; ++it)
    {
        if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
            res.insert(it->second); 
    }

    return flag;
}

bool skhttp_res_data_process::do_check_change_rate_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    float end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();

    std::multimap<float, std::string>::iterator it_le, it_ge, it;
    it_le = p_data->_change_rate_index.current()->end();
    it_ge = p_data->_change_rate_index.current()->begin();

    if (has_key<std::string, std::string>(url_para_map, "change_rate_ge"))
    {
        flag = true;
        end = atof(url_para_map["change_rate_ge"].c_str());
        it_ge = p_data->_change_rate_index.current()->lower_bound(end);

        if (SETS_OP_UNION == et)
            res = search;
    }

    for (it = it_ge; it != it_le && flag; ++it)
    {
        if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
            res.insert(it->second); 
    }

    return flag;
}

bool skhttp_res_data_process::do_check_range_percent_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    float end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();

    std::multimap<float, std::string>::iterator it_le, it_ge, it;
    it_le = p_data->_range_percent_index.current()->end();
    it_ge = p_data->_range_percent_index.current()->begin();

    if (has_key<std::string, std::string>(url_para_map, "range_percent_le"))
    {
        flag = true;
        end = atof(url_para_map["range_percent_le"].c_str());
        it_le = p_data->_range_percent_index.current()->upper_bound(end);

        if (SETS_OP_UNION == et)
            res = search;
    }

    for (it = it_ge; it != it_le && flag; ++it)
    {
        if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
            res.insert(it->second); 
    }

    return flag;
}

bool skhttp_res_data_process::do_check_history_sum_range_percent_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    float end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();
    std::vector<std::string> tmp_vec;
    std::vector<std::set<std::string> > tmp_res_vec;

    std::multimap<float, std::string>::iterator it_le, it_ge, it;

    if (has_key<std::string, std::string>(url_para_map, "history_sum_range_percent_le"))
    {
        flag = true;
        SplitString(url_para_map["history_sum_range_percent_le"].c_str(), '|', &tmp_vec, SPLIT_MODE_ALL);
        if (!tmp_vec.size())
            tmp_vec.push_back(url_para_map["history_sum_range_percent_le"]);

        if (SETS_OP_UNION == et)
            res = search;

        for (uint32_t i = 0; i< tmp_vec.size(); i++) 
        {

            std::vector<std::string> t_vec;
            SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
            std::string date;
            p_data->_hquoation_dict->current()->get_last_date(atoi(t_vec[0].c_str()), date);
            if (date.empty())
                return flag;


            end = atof(t_vec[1].c_str());
            auto ii = p_data->_hq_sum_range_percent_index.current()->find(date);
            if (ii == p_data->_hq_sum_range_percent_index.current()->end())
                return flag;

            it_le = ii->second.end();
            it_ge = ii->second.begin();

            it_le = ii->second.upper_bound(end);
            std::set<std::string> t_res;
            for (it = it_ge; it != it_le; ++it)
            {
                if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
                    t_res.insert(it->second);
            }

            tmp_res_vec.push_back(t_res);
        }
    }

    if (flag)
        get_intersection(tmp_res_vec, res);

    return flag;
}


bool skhttp_res_data_process::do_check_history_has_range_percent_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    float end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();
    std::vector<std::string> tmp_vec;
    std::vector<std::set<std::string> > tmp_res_vec;

    std::multimap<float, std::string>::iterator it_le, it_ge, it;

    if (has_key<std::string, std::string>(url_para_map, "history_has_range_percent_le"))
    {
        flag = true;
        SplitString(url_para_map["history_has_range_percent_le"].c_str(), '|', &tmp_vec, SPLIT_MODE_ALL);
        if (!tmp_vec.size())
            tmp_vec.push_back(url_para_map["history_has_range_percent_le"]);

        if (SETS_OP_UNION == et)
            res = search;

        for (uint32_t i = 0; i< tmp_vec.size(); i++) 
        {

            std::vector<std::string> t_vec;
            SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
            std::string date;
            p_data->_hquoation_dict->current()->get_last_date(atoi(t_vec[0].c_str()), date);
            if (date.empty())
                return flag;


            end = atof(t_vec[1].c_str());
            auto ii = p_data->_hqrange_percent_index.current()->find(date);
            if (ii == p_data->_hqrange_percent_index.current()->end())
                return flag;

            std::set<std::string> t_res;
            for (; ii != p_data->_hqrange_percent_index.current()->end(); ii++)
            {
                it_le = ii->second.end();
                it_ge = ii->second.begin();

                it_le = ii->second.upper_bound(end);
                for (it = it_ge; it != it_le; ++it)
                {
                    if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
                        t_res.insert(it->second); 
                }
            }

            tmp_res_vec.push_back(t_res);
        }
    }

    if (flag)
        get_intersection(tmp_res_vec, res);

    return flag;
}

bool skhttp_res_data_process::do_check_history_range_percent_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    float end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();
    std::vector<std::string> tmp_vec;
    std::vector<std::set<std::string> > tmp_res_vec;

    std::multimap<float, std::string>::iterator it_le, it_ge, it;

    if (has_key<std::string, std::string>(url_para_map, "history_range_percent_le"))
    {
        flag = true;
        SplitString(url_para_map["history_range_percent_le"].c_str(), '|', &tmp_vec, SPLIT_MODE_ALL);
        if (!tmp_vec.size())
            tmp_vec.push_back(url_para_map["history_range_percent_le"]);

        if (SETS_OP_UNION == et)
            res = search;

        for (uint32_t i = 0; i< tmp_vec.size(); i++) 
        {

            std::vector<std::string> t_vec;
            SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
            std::string date;
            p_data->_hquoation_dict->current()->get_last_date(atoi(t_vec[0].c_str()), date);
            if (date.empty())
                return flag;


            end = atof(t_vec[1].c_str());
            auto ii = p_data->_hqrange_percent_index.current()->find(date);
            if (ii == p_data->_hqrange_percent_index.current()->end())
                return flag;

            std::set<std::string> t_res;
            it_le = ii->second.end();
            it_ge = ii->second.begin();

            it_le = ii->second.upper_bound(end);
            for (it = it_ge; it != it_le; ++it)
            {
                if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
                    t_res.insert(it->second); 
            }

            tmp_res_vec.push_back(t_res);
        }
    }

    if (flag)
        get_intersection(tmp_res_vec, res);

    return flag;
}

bool skhttp_res_data_process::do_check_range_percent_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    float end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();

    std::multimap<float, std::string>::iterator it_le, it_ge, it;
    it_le = p_data->_range_percent_index.current()->end();
    it_ge = p_data->_range_percent_index.current()->begin();

    if (has_key<std::string, std::string>(url_para_map, "range_percent_ge"))
    {
        flag = true;
        end = atof(url_para_map["range_percent_ge"].c_str());
        it_ge = p_data->_range_percent_index.current()->lower_bound(end);

        if (SETS_OP_UNION == et)
            res = search;
    }

    for (it = it_ge; it != it_le && flag; ++it)
    {
        if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
            res.insert(it->second); 
    }

    return flag;
}


bool skhttp_res_data_process::do_check_history_sum_range_percent_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    float end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();
    std::vector<std::string> tmp_vec;
    std::vector<std::set<std::string> > tmp_res_vec;

    std::multimap<float, std::string>::iterator it_le, it_ge, it;

    if (has_key<std::string, std::string>(url_para_map, "history_sum_range_percent_ge"))
    {
        flag = true;
        SplitString(url_para_map["history_sum_range_percent_ge"].c_str(), '|', &tmp_vec, SPLIT_MODE_ALL);
        if (!tmp_vec.size())
            tmp_vec.push_back(url_para_map["history_sum_range_percent_ge"]);

        if (SETS_OP_UNION == et)
            res = search;

        for (uint32_t i = 0; i< tmp_vec.size(); i++) 
        {

            std::vector<std::string> t_vec;
            SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
            std::string date;
            p_data->_hquoation_dict->current()->get_last_date(atoi(t_vec[0].c_str()), date);
            if (date.empty())
                return flag;


            end = atof(t_vec[1].c_str());
            auto ii = p_data->_hq_sum_range_percent_index.current()->find(date);
            if (ii == p_data->_hq_sum_range_percent_index.current()->end())
                return flag;

            it_le = ii->second.end();
            it_ge = ii->second.begin();

            it_ge = ii->second.lower_bound(end);
            std::set<std::string> t_res;
            for (it = it_ge; it != it_le; ++it)
            {
                if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
                    t_res.insert(it->second); 
            }

            tmp_res_vec.push_back(t_res);
        }
    }

    if (flag)
        get_intersection(tmp_res_vec, res);

    return flag;
}

bool skhttp_res_data_process::do_check_end_hqend_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    float end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();
    std::vector<std::string> tmp_vec;
    std::vector<std::set<std::string> > tmp_res_vec;

    std::multimap<float, std::string>::iterator it_le, it_ge, it;

    if (has_key<std::string, std::string>(url_para_map, "end_hqend_ge"))
    {
        flag = true;
        SplitString(url_para_map["end_hqend_ge"].c_str(), '|', &tmp_vec, SPLIT_MODE_ALL);
        if (!tmp_vec.size())
            tmp_vec.push_back(url_para_map["end_hqend_ge"]);

        if (SETS_OP_UNION == et)
            res = search;

        for (uint32_t i = 0; i< tmp_vec.size(); i++) 
        {

            std::vector<std::string> t_vec;
            SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
            std::string date;
            p_data->_hquoation_dict->current()->get_last_date(atoi(t_vec[0].c_str()), date);
            if (date.empty())
                return flag;


            end = atof(t_vec[1].c_str());
            auto ii = p_data->_end_hqend_index.current()->find(date);
            if (ii == p_data->_end_hqend_index.current()->end())
                return flag;

            it_le = ii->second.end();
            it_ge = ii->second.begin();

            it_ge = ii->second.lower_bound(end);
            std::set<std::string> t_res;
            for (it = it_ge; it != it_le; ++it)
            {
                if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
                    t_res.insert(it->second); 
            }

            tmp_res_vec.push_back(t_res);
        }
    }

    if (flag)
        get_intersection(tmp_res_vec, res);

    return flag;
}

bool skhttp_res_data_process::do_check_history_has_range_percent_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    float end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();
    std::vector<std::string> tmp_vec;
    std::vector<std::set<std::string> > tmp_res_vec;

    std::multimap<float, std::string>::iterator it_le, it_ge, it;

    if (has_key<std::string, std::string>(url_para_map, "history_has_range_percent_ge"))
    {
        flag = true;
        SplitString(url_para_map["history_has_range_percent_ge"].c_str(), '|', &tmp_vec, SPLIT_MODE_ALL);
        if (!tmp_vec.size())
            tmp_vec.push_back(url_para_map["history_has_range_percent_ge"]);

        if (SETS_OP_UNION == et)
            res = search;

        for (uint32_t i = 0; i< tmp_vec.size(); i++) 
        {

            std::vector<std::string> t_vec;
            SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
            std::string date;
            p_data->_hquoation_dict->current()->get_last_date(atoi(t_vec[0].c_str()), date);
            if (date.empty())
                return flag;


            end = atof(t_vec[1].c_str());
            auto ii = p_data->_hqrange_percent_index.current()->find(date);
            if (ii == p_data->_hqrange_percent_index.current()->end())
                return flag;

            std::set<std::string> t_res;
            for (; ii != p_data->_hqrange_percent_index.current()->end(); ii++)
            {
                it_le = ii->second.end();
                it_ge = ii->second.begin();

                it_ge = ii->second.lower_bound(end);
                for (it = it_ge; it != it_le; ++it)
                {
                    if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
                        t_res.insert(it->second); 
                }
            }

            tmp_res_vec.push_back(t_res);
        }
    }

    if (flag)
        get_intersection(tmp_res_vec, res);

    return flag;
}

bool skhttp_res_data_process::do_check_history_range_percent_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    float end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();
    std::vector<std::string> tmp_vec;
    std::vector<std::set<std::string> > tmp_res_vec;

    std::multimap<float, std::string>::iterator it_le, it_ge, it;

    if (has_key<std::string, std::string>(url_para_map, "history_range_percent_ge"))
    {
        flag = true;
        SplitString(url_para_map["history_range_percent_ge"].c_str(), '|', &tmp_vec, SPLIT_MODE_ALL);
        if (!tmp_vec.size())
            tmp_vec.push_back(url_para_map["history_range_percent_ge"]);

        if (SETS_OP_UNION == et)
            res = search;

        for (uint32_t i = 0; i< tmp_vec.size(); i++) 
        {

            std::vector<std::string> t_vec;
            SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
            std::string date;
            p_data->_hquoation_dict->current()->get_last_date(atoi(t_vec[0].c_str()), date);
            if (date.empty())
                return flag;


            end = atof(t_vec[1].c_str());
            auto ii = p_data->_hqrange_percent_index.current()->find(date);
            if (ii == p_data->_hqrange_percent_index.current()->end())
                return flag;

            std::set<std::string> t_res;
            it_le = ii->second.end();
            it_ge = ii->second.begin();

            it_ge = ii->second.lower_bound(end);
            for (it = it_ge; it != it_le; ++it)
            {
                if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
                    t_res.insert(it->second); 
            }

            tmp_res_vec.push_back(t_res);
        }
    }

    if (flag)
        get_intersection(tmp_res_vec, res);

    return flag;
}

bool skhttp_res_data_process::do_check_down_pointer_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    float end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();

    std::multimap<float, std::string>::iterator it_le, it_ge, it;
    it_le = p_data->_down_pointer_index.current()->end();
    it_ge = p_data->_down_pointer_index.current()->begin();

    if (has_key<std::string, std::string>(url_para_map, "down_pointer_le"))
    {
        flag = true;
        end = atof(url_para_map["down_pointer_le"].c_str());
        it_le = p_data->_down_pointer_index.current()->upper_bound(end);

        if (SETS_OP_UNION == et)
            res = search;
    }

    for (it = it_ge; it != it_le && flag; ++it)
    {
        if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
            res.insert(it->second); 
    }

    return flag;
}

bool skhttp_res_data_process::do_check_down_pointer_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    float end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();

    std::multimap<float, std::string>::iterator it_le, it_ge, it;
    it_le = p_data->_down_pointer_index.current()->end();
    it_ge = p_data->_down_pointer_index.current()->begin();

    if (has_key<std::string, std::string>(url_para_map, "down_pointer_ge"))
    {
        flag = true;
        end = atof(url_para_map["down_pointer_ge"].c_str());
        it_ge = p_data->_down_pointer_index.current()->lower_bound(end);
        if (SETS_OP_UNION == et)
            res = search;
    }

    for (it = it_ge; it != it_le && flag; ++it)
    {
        if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
            res.insert(it->second); 
    }

    return flag;
}

bool skhttp_res_data_process::do_check_up_pointer_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    float end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();

    std::multimap<float, std::string>::iterator it_le, it_ge, it;
    it_le = p_data->_up_pointer_index.current()->end();
    it_ge = p_data->_up_pointer_index.current()->begin();

    if (has_key<std::string, std::string>(url_para_map, "up_pointer_le"))
    {
        flag = true;
        end = atof(url_para_map["up_pointer_le"].c_str());
        it_le = p_data->_up_pointer_index.current()->upper_bound(end);
        if (SETS_OP_UNION == et)
            res = search;
    }

    for (it = it_ge; it != it_le && flag; ++it)
    {
        if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
            res.insert(it->second); 
    }

    return flag;
}

bool skhttp_res_data_process::do_check_up_pointer_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    float end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();

    std::multimap<float, std::string>::iterator it_le, it_ge, it;
    it_le = p_data->_up_pointer_index.current()->end();
    it_ge = p_data->_up_pointer_index.current()->begin();

    if (has_key<std::string, std::string>(url_para_map, "up_pointer_ge"))
    {
        flag = true;
        end = atof(url_para_map["up_pointer_ge"].c_str());
        it_ge = p_data->_up_pointer_index.current()->lower_bound(end);
        if (SETS_OP_UNION == et)
            res = search;
    }

    for (it = it_ge; it != it_le && flag; ++it)
    {
        if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
            res.insert(it->second); 
    }

    return flag;
}

bool skhttp_res_data_process::do_check_end_avg_price_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    float end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();

    std::multimap<float, std::string>::iterator it_le, it_ge, it;
    it_le = p_data->_end_avg_price_index.current()->end();
    it_ge = p_data->_end_avg_price_index.current()->begin();

    if (has_key<std::string, std::string>(url_para_map, "end_avg_price_le"))
    {
        flag = true;
        end = atof(url_para_map["end_avg_price_le"].c_str());
        it_le = p_data->_end_avg_price_index.current()->upper_bound(end);
        if (SETS_OP_UNION == et)
            res = search;
    }

    for (it = it_ge; it != it_le && flag; ++it)
    {
        if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
            res.insert(it->second); 
    }

    return flag;
}

bool skhttp_res_data_process::do_check_end_avg_price_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    float end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();

    std::multimap<float, std::string>::iterator it_le, it_ge, it;
    it_le = p_data->_end_avg_price_index.current()->end();
    it_ge = p_data->_end_avg_price_index.current()->begin();

    if (has_key<std::string, std::string>(url_para_map, "end_avg_price_ge"))
    {
        flag = true;
        end = atof(url_para_map["end_avg_price_ge"].c_str());
        it_ge = p_data->_end_avg_price_index.current()->lower_bound(end);
        if (SETS_OP_UNION == et)
            res = search;
    }

    for (it = it_ge; it != it_le && flag; ++it)
    {
        if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
            res.insert(it->second); 
    }

    return flag;
}

bool skhttp_res_data_process::do_check_pe_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    int end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();

    std::multimap<int, std::string>::iterator it_le, it_ge, it;
    it_le = p_data->_pe_index.current()->end();
    it_ge = p_data->_pe_index.current()->begin();

    if (has_key<std::string, std::string>(url_para_map, "pe_le"))
    {
        flag = true;
        end = atoi(url_para_map["pe_le"].c_str());
        it_le = p_data->_pe_index.current()->upper_bound(end);
        if (SETS_OP_UNION == et)
            res = search;
    }

    for (it = it_ge; it != it_le && flag; ++it)
    {
        if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
            res.insert(it->second); 
    }

    return flag;
}


bool skhttp_res_data_process::do_check_pe_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    int end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();

    std::multimap<int, std::string>::iterator it_le, it_ge, it;
    it_le = p_data->_pe_index.current()->end();
    it_ge = p_data->_pe_index.current()->begin();

    if (has_key<std::string, std::string>(url_para_map, "pe_ge"))
    {
        flag = true;
        end = atoi(url_para_map["pe_ge"].c_str());
        it_ge = p_data->_pe_index.current()->lower_bound(end);
        if (SETS_OP_UNION == et)
            res = search;
    }

    for (it = it_ge; it != it_le && flag; ++it)
    {
        if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
            res.insert(it->second); 
    }

    return flag;
}

bool skhttp_res_data_process::do_check_pb_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    int end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();

    std::multimap<int, std::string>::iterator it_le, it_ge, it;
    it_le = p_data->_pb_index.current()->end();
    it_ge = p_data->_pb_index.current()->begin();

    if (has_key<std::string, std::string>(url_para_map, "pb_le"))
    {
        flag = true;
        end = atoi(url_para_map["pb_le"].c_str());
        it_le = p_data->_pb_index.current()->upper_bound (end);
        if (SETS_OP_UNION == et)
            res = search;
    }

    for (it = it_ge; it != it_le && flag; ++it)
    {
        if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
            res.insert(it->second); 
    }

    return flag;
}

bool skhttp_res_data_process::do_check_pb_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    int end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();

    std::multimap<int, std::string>::iterator it_le, it_ge, it;
    it_le = p_data->_pb_index.current()->end();
    it_ge = p_data->_pb_index.current()->begin();

    if (has_key<std::string, std::string>(url_para_map, "pb_ge"))
    {
        flag = true;
        end = atoi(url_para_map["pb_ge"].c_str());
        it_ge = p_data->_pb_index.current()->lower_bound(end);
        if (SETS_OP_UNION == et)
            res = search;
    }

    for (it = it_ge; it != it_le && flag; ++it)
    {
        if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
            res.insert(it->second); 
    }

    return flag;
}

bool skhttp_res_data_process::do_check_value_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    int end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();

    std::multimap<int, std::string>::iterator it_le, it_ge, it;
    it_le = p_data->_value_index.current()->end();
    it_ge = p_data->_value_index.current()->begin();

    if (has_key<std::string, std::string>(url_para_map, "value_le"))
    {
        flag = true;
        end = atoi(url_para_map["value_le"].c_str());
        it_le = p_data->_value_index.current()->upper_bound(end);
        if (SETS_OP_UNION == et)
            res = search;
    }

    for (it = it_ge; it != it_le && flag; ++it)
    {
        if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
            res.insert(it->second); 
    }

    return flag;
}

bool skhttp_res_data_process::do_check_value_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    int end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();

    std::multimap<int, std::string>::iterator it_le, it_ge, it;
    it_le = p_data->_value_index.current()->end();
    it_ge = p_data->_value_index.current()->begin();

    if (has_key<std::string, std::string>(url_para_map, "value_ge"))
    {
        flag = true;
        end = atoi(url_para_map["value_ge"].c_str());
        it_ge = p_data->_value_index.current()->lower_bound(end);
        if (SETS_OP_UNION == et)
            res = search;
    }

    for (it = it_ge; it != it_le && flag; ++it)
    {
        if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
            res.insert(it->second); 
    }

    return flag;
}

bool skhttp_res_data_process::do_check_cir_value_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    int end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();

    std::multimap<int, std::string>::iterator it_le, it_ge, it;
    it_le = p_data->_cir_value_index.current()->end();
    it_ge = p_data->_cir_value_index.current()->begin();

    if (has_key<std::string, std::string>(url_para_map, "cir_value_le"))
    {
        flag = true;
        end = atoi(url_para_map["cir_value_le"].c_str());
        it_le = p_data->_cir_value_index.current()->upper_bound(end);
        if (SETS_OP_UNION == et)
            res = search;
    }

    for (it = it_ge; it != it_le && flag; ++it)
    {
        if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
            res.insert(it->second); 
    }

    return flag;
}

bool skhttp_res_data_process::do_check_cir_value_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    int end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();

    std::multimap<int, std::string>::iterator it_le, it_ge, it;
    it_le = p_data->_cir_value_index.current()->end();
    it_ge = p_data->_cir_value_index.current()->begin();

    if (has_key<std::string, std::string>(url_para_map, "cir_value_ge"))
    {
        flag = true;
        end = atoi(url_para_map["cir_value_ge"].c_str());
        it_ge = p_data->_cir_value_index.current()->lower_bound(end);
        if (SETS_OP_UNION == et)
            res = search;
    }

    for (it = it_ge; it != it_le && flag; ++it)
    {
        if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
            res.insert(it->second); 
    }

    return flag;
}

bool skhttp_res_data_process::do_check_cir_value_num_min(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    int end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();
    int i = 0;

    if (has_key<std::string, std::string>(url_para_map, "cir_value_min"))
    {
        flag = true;
        end = atoi(url_para_map["cir_value_min"].c_str());
        i = 0;
        if (SETS_OP_UNION == et)
            res = search;

        for (auto ii = p_data->_cir_value_index.current()->begin();
                ii != p_data->_cir_value_index.current()->end() && i < end; ii++, i++)
        {
            if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(ii->second)) || SETS_OP_UNION == et)
                res.insert(ii->second); 
        }
    }

    return flag;
}

bool skhttp_res_data_process::do_check_cir_value_num_max(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    int end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();
    int i = 0;

    if (has_key<std::string, std::string>(url_para_map, "cir_value_max"))
    {
        flag = true;
        end = atoi(url_para_map["cir_value_max"].c_str());
        i = 0;
        if (SETS_OP_UNION == et)
            res = search;

        for (auto ii = p_data->_cir_value_index.current()->rbegin();
                ii != p_data->_cir_value_index.current()->rend() && i < end; ii++, i++)
        {
            if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(ii->second)) || SETS_OP_UNION == et)
                res.insert(ii->second); 
        }
    }

    return flag;
}

bool skhttp_res_data_process::do_check_value_num_min(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    int end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();
    int i = 0;

    if (has_key<std::string, std::string>(url_para_map, "value_min"))
    {
        flag = true;
        end = atoi(url_para_map["value_min"].c_str());
        i = 0;
        if (SETS_OP_UNION == et)
            res = search;

        for (auto ii = p_data->_cir_value_index.current()->begin();
                ii != p_data->_cir_value_index.current()->end() && i < end; ii++, i++)
        {
            if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(ii->second)) || SETS_OP_UNION == et)
                res.insert(ii->second); 
        }
    }

    return flag;
}

bool skhttp_res_data_process::do_check_value_num_max(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res,  std::set<std::string> & search, SETS_OP_TRPE et)
{
    int end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();
    int i = 0;

    if (has_key<std::string, std::string>(url_para_map, "value_max"))
    {
        flag = true;
        end = atoi(url_para_map["value_max"].c_str());
        i = 0;
        if (SETS_OP_UNION == et)
            res = search;

        for (auto ii = p_data->_cir_value_index.current()->rbegin();
                ii != p_data->_cir_value_index.current()->rend() && i < end; ii++, i++)
        {
            if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(ii->second)) || SETS_OP_UNION == et)
                res.insert(ii->second); 
        }
    }

    return flag;
}

bool skhttp_res_data_process::do_check_mgxj_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    float end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();

    std::multimap<float, std::string>::iterator it_le, it_ge, it;
    it_le = p_data->_mgxj_index.current()->end();
    it_ge = p_data->_mgxj_index.current()->begin();

    if (has_key<std::string, std::string>(url_para_map, "mgxj_le"))
    {
        flag = true;
        end = atof(url_para_map["mgxj_le"].c_str());
        it_le = p_data->_mgxj_index.current()->upper_bound(end);
        if (SETS_OP_UNION == et)
            res = search;
    }

    for (it = it_ge; it != it_le && flag; ++it)
    {
        if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
            res.insert(it->second); 
    }

    return flag;
}

bool skhttp_res_data_process::do_check_mgxj_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    float end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();

    std::multimap<float, std::string>::iterator it_le, it_ge, it;
    it_le = p_data->_mgxj_index.current()->end();
    it_ge = p_data->_mgxj_index.current()->begin();

    if (has_key<std::string, std::string>(url_para_map, "mgxj_ge"))
    {
        flag = true;
        end = atof(url_para_map["mgxj_ge"].c_str());
        it_ge = p_data->_mgxj_index.current()->lower_bound(end);
        if (SETS_OP_UNION == et)
            res = search;
    }

    for (it = it_ge; it != it_le && flag; ++it)
    {
        if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
            res.insert(it->second); 
    }

    return flag;
}

bool skhttp_res_data_process::do_check_mgsy_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{

    float end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();

    std::multimap<float, std::string>::iterator it_le, it_ge, it;
    it_le = p_data->_mgsy_index.current()->end();
    it_ge = p_data->_mgsy_index.current()->begin();

    if (has_key<std::string, std::string>(url_para_map, "mgsy_le"))
    {
        flag = true;
        end = atof(url_para_map["mgsy_le"].c_str());
        it_le = p_data->_mgsy_index.current()->upper_bound(end);
        if (SETS_OP_UNION == et)
            res = search;
    }

    for (it = it_ge; it != it_le && flag; ++it)
    {
        if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
            res.insert(it->second); 
    }

    return flag;
}

bool skhttp_res_data_process::do_check_mgsy_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{

    float end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();

    std::multimap<float, std::string>::iterator it_le, it_ge, it;
    it_le = p_data->_mgsy_index.current()->end();
    it_ge = p_data->_mgsy_index.current()->begin();

    if (has_key<std::string, std::string>(url_para_map, "mgsy_ge"))
    {
        flag = true;
        end = atof(url_para_map["mgsy_ge"].c_str());
        it_ge = p_data->_mgsy_index.current()->lower_bound(end);
        if (SETS_OP_UNION == et)
            res = search;
    }

    for (it = it_ge; it != it_le && flag; ++it)
    {
        if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
            res.insert(it->second); 
    }

    return flag;
}

bool skhttp_res_data_process::do_check_mgsygr_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    float end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();

    std::multimap<float, std::string>::iterator it_le, it_ge, it;
    it_le = p_data->_mgsygr_index.current()->end();
    it_ge = p_data->_mgsygr_index.current()->begin();

    if (has_key<std::string, std::string>(url_para_map, "mgsygr_le"))
    {
        flag = true;
        end = atof(url_para_map["mgsygr_le"].c_str());
        it_le = p_data->_mgsygr_index.current()->upper_bound(end);
        if (SETS_OP_UNION == et)
            res = search;
    }

    for (it = it_ge; it != it_le && flag; ++it)
    {
        if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
            res.insert(it->second); 
    }

    return flag;
}

bool skhttp_res_data_process::do_check_mgsygr_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    float end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();

    std::multimap<float, std::string>::iterator it_le, it_ge, it;
    it_le = p_data->_mgsygr_index.current()->end();
    it_ge = p_data->_mgsygr_index.current()->begin();

    if (has_key<std::string, std::string>(url_para_map, "mgsygr_ge"))
    {
        flag = true;
        end = atof(url_para_map["mgsygr_ge"].c_str());
        it_ge = p_data->_mgsygr_index.current()->lower_bound(end);
        if (SETS_OP_UNION == et)
            res = search;
    }

    for (it = it_ge; it != it_le && flag; ++it)
    {
        if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
            res.insert(it->second); 
    }

    return flag;
}

bool skhttp_res_data_process::do_check_mgxjgr_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    float end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();

    std::multimap<float, std::string>::iterator it_le, it_ge, it;
    it_le = p_data->_mgxjgr_index.current()->end();
    it_ge = p_data->_mgxjgr_index.current()->begin();

    if (has_key<std::string, std::string>(url_para_map, "mgxjgr_le"))
    {
        flag = true;
        end = atof(url_para_map["mgxjgr_le"].c_str());
        it_le = p_data->_mgxjgr_index.current()->upper_bound(end);
        if (SETS_OP_UNION == et)
            res = search;
    }

    for (it = it_ge; it != it_le && flag; ++it)
    {
        if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
            res.insert(it->second); 
    }

    return flag;
}

bool skhttp_res_data_process::do_check_mgxjgr_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    float end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();

    std::multimap<float, std::string>::iterator it_le, it_ge, it;
    it_le = p_data->_mgxjgr_index.current()->end();
    it_ge = p_data->_mgxjgr_index.current()->begin();

    if (has_key<std::string, std::string>(url_para_map, "mgxjgr_ge"))
    {
        flag = true;
        end = atof(url_para_map["mgxjgr_ge"].c_str());
        it_ge = p_data->_mgxjgr_index.current()->lower_bound(end);
        if (SETS_OP_UNION == et)
            res = search;
    }

    for (it = it_ge; it != it_le && flag; ++it)
    {
        if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
            res.insert(it->second); 
    }

    return flag;
}

bool skhttp_res_data_process::do_check_zysrgr_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    float end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();

    std::multimap<float, std::string>::iterator it_le, it_ge, it;
    it_le = p_data->_zysrgr_index.current()->end();
    it_ge = p_data->_zysrgr_index.current()->begin();

    if (has_key<std::string, std::string>(url_para_map, "zysrgr_le"))
    {
        flag = true;
        end = atof(url_para_map["zysrgr_le"].c_str());
        it_le = p_data->_zysrgr_index.current()->upper_bound(end);
        if (SETS_OP_UNION == et)
            res = search;
    }

    for (it = it_ge; it != it_le && flag; ++it)
    {
        if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
            res.insert(it->second); 
    }

    return flag;
}

bool skhttp_res_data_process::do_check_zysrgr_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    float end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();

    std::multimap<float, std::string>::iterator it_le, it_ge, it;
    it_le = p_data->_zysrgr_index.current()->end();
    it_ge = p_data->_zysrgr_index.current()->begin();

    if (has_key<std::string, std::string>(url_para_map, "zysrgr_ge"))
    {
        flag = true;
        end = atof(url_para_map["zysrgr_ge"].c_str());
        it_ge = p_data->_zysrgr_index.current()->lower_bound(end);
        if (SETS_OP_UNION == et)
            res = search;
    }

    for (it = it_ge; it != it_le && flag; ++it)
    {
        if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
            res.insert(it->second); 
    }

    return flag;
}

bool skhttp_res_data_process::do_check_yylrgr_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    float end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();

    std::multimap<float, std::string>::iterator it_le, it_ge, it;
    it_le = p_data->_yylrgr_index.current()->end();
    it_ge = p_data->_yylrgr_index.current()->begin();

    if (has_key<std::string, std::string>(url_para_map, "yylrgr_le"))
    {
        flag = true;
        end = atof(url_para_map["yylrgr_le"].c_str());
        it_le = p_data->_yylrgr_index.current()->upper_bound(end);
        if (SETS_OP_UNION == et)
            res = search;
    }

    for (it = it_ge; it != it_le && flag; ++it)
    {
        if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
            res.insert(it->second); 
    }

    return flag;
}

bool skhttp_res_data_process::do_check_yylrgr_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    float end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();

    std::multimap<float, std::string>::iterator it_le, it_ge, it;
    it_le = p_data->_yylrgr_index.current()->end();
    it_ge = p_data->_yylrgr_index.current()->begin();

    if (has_key<std::string, std::string>(url_para_map, "yylrgr_ge"))
    {
        flag = true;
        end = atof(url_para_map["yylrgr_ge"].c_str());
        it_ge = p_data->_yylrgr_index.current()->lower_bound(end);
        if (SETS_OP_UNION == et)
            res = search;
    }

    for (it = it_ge; it != it_le && flag; ++it)
    {
        if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
            res.insert(it->second); 
    }

    return flag;
}

bool skhttp_res_data_process::do_check_id_substr(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)  
{
    bool flag = false;
    proc_data* p_data = proc_data::instance();

    if (has_key<std::string, std::string>(url_para_map, "id_substr"))
    {
        flag = true;
        if (SETS_OP_UNION == et)
            res = search;
    }

    id_dict * id_dic = p_data->_id_dict->current();

    for (auto ii = id_dic->_id_vec.begin(); ii != id_dic->_id_vec.end() && flag; ii++)
    {
        if (strstr(ii->c_str(), url_para_map["id_substr"].c_str()))
        {
            if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(*ii)) || SETS_OP_UNION == et)
                res.insert(*ii);
        }
    }

    return flag;
}

bool skhttp_res_data_process::do_check_jlrgr_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    float end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();

    std::multimap<float, std::string>::iterator it_le, it_ge, it;
    it_le = p_data->_jlrgr_index.current()->end();
    it_ge = p_data->_jlrgr_index.current()->begin();

    if (has_key<std::string, std::string>(url_para_map, "jlrgr_le"))
    {
        flag = true;
        end = atof(url_para_map["jlrgr_le"].c_str());
        it_le = p_data->_jlrgr_index.current()->upper_bound(end);
        if (SETS_OP_UNION == et)
            res = search;
    }

    for (it = it_ge; it != it_le && flag; ++it)
    {
        if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
            res.insert(it->second); 
    }

    return flag;
}

bool skhttp_res_data_process::do_check_jlrgr_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    float end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();

    std::multimap<float, std::string>::iterator it_le, it_ge, it;
    it_le = p_data->_jlrgr_index.current()->end();
    it_ge = p_data->_jlrgr_index.current()->begin();

    if (has_key<std::string, std::string>(url_para_map, "jlrgr_ge"))
    {
        flag = true;
        end = atof(url_para_map["jlrgr_ge"].c_str());
        it_ge = p_data->_jlrgr_index.current()->lower_bound(end);
        if (SETS_OP_UNION == et)
            res = search;
    }

    for (it = it_ge; it != it_le && flag; ++it)
    {
        if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
            res.insert(it->second); 
    }

    return flag;
}

bool skhttp_res_data_process::do_check_address(std::map<std::string, std::string> & url_para_map, const char * key, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    proc_data* p_data = proc_data::instance();
    bool flag = false;
    std::vector<std::string> tmp_vec;

    if (key && has_key<std::string, std::string>(url_para_map, key))
    {
        flag = true;
        SplitString(url_para_map[key].c_str(), '|', &tmp_vec, SPLIT_MODE_ALL);
        if (!tmp_vec.size()) 
            tmp_vec.push_back(url_para_map[key]);

        if (SETS_OP_UNION == et)
            res = search;

        for (uint32_t i = 0; i< tmp_vec.size(); i++) {

            std::shared_ptr<std::string> ss(new std::string(trim(tmp_vec[i].c_str())));
            auto range = p_data->_address_index.current()->equal_range(ss);
            for (auto it = range.first; it != range.second; ++it)
            {
                if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
                    res.insert(it->second);
            }
        }
    }

    return flag;
}

bool skhttp_res_data_process::do_check_plate(std::map<std::string, std::string> & url_para_map, const char * key, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    proc_data* p_data = proc_data::instance();
    bool flag = false;
    std::vector<std::string> tmp_vec;

    if (key && has_key<std::string, std::string>(url_para_map, key))
    {
        flag = true;
        SplitString(url_para_map[key].c_str(), '|', &tmp_vec, SPLIT_MODE_ALL);
        if (!tmp_vec.size()) 
            tmp_vec.push_back(url_para_map[key]);

        if (SETS_OP_UNION == et)
            res = search;

        for (uint32_t i = 0; i< tmp_vec.size(); i++) {
            std::shared_ptr<std::string> ss(new std::string(trim(tmp_vec[i].c_str())));
            auto range = p_data->_plate_index.current()->equal_range(ss);
            for (auto it = range.first; it != range.second; ++it)
            {
                if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
                    res.insert(it->second);
            }
        }
    }

    return flag;
}

bool skhttp_res_data_process::do_check_rsingle_diff2_ge(std::map<std::string, std::string> & url_para_map, uint32_t index, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{

    int end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();
    char t_buf[SIZE_LEN_128];

    std::multimap<int, std::string>::iterator it_le, it_ge, it;

    if (index >= p_data->_rsingle_diff2_index.current()->size())
        return flag;

    it_le = p_data->_rsingle_diff2_index.current()->at(index).end();
    it_ge = p_data->_rsingle_diff2_index.current()->at(index).begin();

    snprintf(t_buf, sizeof(t_buf), "rsingle_diff2_%d_ge", index);
    if (has_key<std::string, std::string>(url_para_map, t_buf))
    {
        flag = true;
        end = atoi(url_para_map[t_buf].c_str());
        it_ge = p_data->_rsingle_diff2_index.current()->at(index).lower_bound(end);

        if (SETS_OP_UNION == et)
            res = search;
    }

    for (it = it_ge; it != it_le && flag; ++it)
    {
        if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
            res.insert(it->second); 
    }

    return flag;
}

bool skhttp_res_data_process::do_check_rsingle_diff_ge(std::map<std::string, std::string> & url_para_map, uint32_t index, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{

    int end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();
    char t_buf[SIZE_LEN_128];

    std::multimap<int, std::string>::iterator it_le, it_ge, it;

    if (index >= p_data->_rsingle_diff_index.current()->size())
        return flag;

    it_le = p_data->_rsingle_diff_index.current()->at(index).end();
    it_ge = p_data->_rsingle_diff_index.current()->at(index).begin();

    snprintf(t_buf, sizeof(t_buf), "rsingle_diff_%d_ge", index);
    if (has_key<std::string, std::string>(url_para_map, t_buf))
    {
        flag = true;
        end = atoi(url_para_map[t_buf].c_str());
        it_ge = p_data->_rsingle_diff_index.current()->at(index).lower_bound(end);

        if (SETS_OP_UNION == et)
            res = search;
    }

    for (it = it_ge; it != it_le && flag; ++it)
    {
        if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
            res.insert(it->second); 
    }

    return flag;
}

bool skhttp_res_data_process::do_check_hsingle_sum_diff_ge(std::map<std::string, std::string> & url_para_map, uint32_t index, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    int end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();
    char t_buf[SIZE_LEN_128]; 

    std::vector<std::string> tmp_vec;
    std::vector<std::set<std::string> > tmp_res_vec;

    std::multimap<int, std::string>::iterator it_le, it_ge, it;
    if (index >= p_data->_rsingle_diff_index.current()->size())
        return flag;

    snprintf(t_buf, sizeof(t_buf), "hsingle_sum_diff_%d_ge", index);

    if (has_key<std::string, std::string>(url_para_map, t_buf))
    {
        flag = true;
        SplitString(t_buf, '|', &tmp_vec, SPLIT_MODE_ALL);
        if (!tmp_vec.size())
            tmp_vec.push_back(url_para_map[t_buf]);

        if (SETS_OP_UNION == et)
            res = search;

        for (uint32_t i = 0; i< tmp_vec.size(); i++) 
        {

            std::vector<std::string> t_vec;
            SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
            std::string date;
            p_data->_hsingle_dict->current()->get_last_date(atoi(t_vec[0].c_str()), date);
            if (date.empty())
                return flag;

            end = atoi(t_vec[1].c_str());
            auto ii = p_data->_hsingle_sum_diff_index.current()->at(index).find(date);
            if (ii == p_data->_hsingle_sum_diff_index.current()->at(index).end())
                return flag;

            it_le = ii->second.end();
            it_ge = ii->second.begin();

            it_ge = ii->second.lower_bound(end);
            std::set<std::string> t_res;
            for (it = it_ge; it != it_le; ++it)
            {
                if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
                    t_res.insert(it->second); 
            }

            tmp_res_vec.push_back(t_res);
        }
    }

    if (flag)
        get_intersection(tmp_res_vec, res);

    return flag;
}


bool skhttp_res_data_process::do_check_hsingle_diff_ge(std::map<std::string, std::string> & url_para_map, uint32_t index, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    int end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();
    char t_buf[SIZE_LEN_128]; 

    std::vector<std::string> tmp_vec;
    std::vector<std::set<std::string> > tmp_res_vec;

    std::multimap<int, std::string>::iterator it_le, it_ge, it;
    if (index >= p_data->_rsingle_diff_index.current()->size())
        return flag;

    snprintf(t_buf, sizeof(t_buf), "hsingle_diff_%d_ge", index);

    if (has_key<std::string, std::string>(url_para_map, t_buf))
    {
        flag = true;
        SplitString(t_buf, '|', &tmp_vec, SPLIT_MODE_ALL);
        if (!tmp_vec.size())
            tmp_vec.push_back(url_para_map[t_buf]);

        if (SETS_OP_UNION == et)
            res = search;

        for (uint32_t i = 0; i< tmp_vec.size(); i++) 
        {

            std::vector<std::string> t_vec;
            SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
            std::string date;
            p_data->_hsingle_dict->current()->get_last_date(atoi(t_vec[0].c_str()), date);
            if (date.empty())
                return flag;


            end = atoi(t_vec[1].c_str());
            auto ii = p_data->_hsingle_diff_index.current()->at(index).find(date);
            if (ii == p_data->_hsingle_diff_index.current()->at(index).end())
                return flag;

            it_le = ii->second.end();
            it_ge = ii->second.begin();

            it_ge = ii->second.lower_bound(end);
            std::set<std::string> t_res;
            for (it = it_ge; it != it_le; ++it)
            {
                if (search.empty() || (SETS_OP_INTERSECTION == et && search.count(it->second)) || SETS_OP_UNION == et)
                    t_res.insert(it->second); 
            }

            tmp_res_vec.push_back(t_res);
        }
    }

    if (flag)
        get_intersection(tmp_res_vec, res);

    return flag;
}

void skhttp_res_data_process::get_intersection(std::vector<std::set<std::string> > & arr, std::set<std::string> &res)
{
    std::set<std::string> search, tmp;
    auto ii = arr.begin();
    if (arr.size())
    {
        search = (*ii);
        ii++;
    }

    for (; ii != arr.end(); ii++)
    {
        tmp.clear();

        std::set_intersection(search.begin(), search.end(), ii->begin(), ii->end(), std::inserter(tmp,tmp.begin()));
        if (tmp.empty())
        {
            return;
        }

        search = tmp;
    }

    res = search;
}

void skhttp_res_data_process::get_union(std::vector<std::set<std::string> > & arr, std::set<std::string> &res)
{
    std::set<std::string> search, tmp;
    auto ii = arr.begin();
    if (arr.size())
    {
        search = (*ii);
        ii++;
    }

    for (; ii != arr.end(); ii++)
    {
        tmp.clear();

        std::set_union(search.begin(), search.end(), ii->begin(), ii->end(), std::inserter(tmp,tmp.begin()));

        if (tmp.empty())
        {
            return;
        }

        search = tmp;
    }

    res = search;

}

int skhttp_res_data_process::do_check_select(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res)
{
    proc_data* p_data = proc_data::instance();
    if (!p_data)
        return -1;


    std::set<std::string> positive, negative;

    {
        std::set<std::string> tmp;
        if (do_check_end_le(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_end_ge(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;

            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_change_rate_le(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_change_rate_ge(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_range_percent_le(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_history_sum_range_percent_le(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_history_has_range_percent_le(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_history_range_percent_le(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }


    {
        std::set<std::string> tmp;
        if (do_check_range_percent_ge(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_history_sum_range_percent_ge(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_history_has_range_percent_ge(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_end_hqend_ge(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_history_range_percent_ge(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }


    {
        std::set<std::string> tmp;
        if (do_check_down_pointer_le(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_down_pointer_ge(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_up_pointer_le(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_up_pointer_ge(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_end_avg_price_le(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_end_avg_price_ge(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_pe_le(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_pe_ge(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_pb_le(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_pb_ge(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_value_le(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_value_ge(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_value_num_min(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_value_num_max(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_cir_value_le(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_cir_value_ge(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_cir_value_num_min(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_cir_value_num_max(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_mgxj_le(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_mgxj_ge(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_mgsy_le(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_mgsy_ge(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_mgsygr_le(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_mgsygr_ge(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_mgxjgr_le(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_mgxjgr_ge(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_zysrgr_le(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_zysrgr_ge(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_yylrgr_le(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_yylrgr_ge(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_jlrgr_le(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_jlrgr_ge(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_mgxj_le(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_mgxj_ge(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_mgsy_le(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_mgsy_ge(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_mgsygr_le(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_mgsygr_ge(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_mgxjgr_le(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_mgxjgr_ge(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_zysrgr_le(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_zysrgr_ge(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_yylrgr_le(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_yylrgr_ge(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_jlrgr_le(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }


    {
        std::set<std::string> tmp;
        if (do_check_id_substr(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_jlrgr_ge(url_para_map, tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }


    {
        std::set<std::string> tmp;
        if (do_check_address(url_para_map, "address", tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_address(url_para_map, "address_v", tmp, negative, SETS_OP_UNION))
        {
            negative = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_plate(url_para_map, "plate", tmp, positive, SETS_OP_INTERSECTION))
        {
            if (tmp.empty())
            {
                return -1;
            }
            positive = tmp;
        }
    }

    {
        std::set<std::string> tmp;
        if (do_check_plate(url_para_map, "plate_v", tmp, negative, SETS_OP_UNION))
        {
            negative = tmp;
        }
    }

    {
        strategy_conf * strategy = p_data->_conf->_strategy->current();
        for (uint32_t i = 0; i < strategy->real_single_scale.size(); i++)
        {
            {
                std::set<std::string> tmp;
                if (do_check_rsingle_diff2_ge(url_para_map, i, tmp, positive, SETS_OP_INTERSECTION))
                {
                    if (tmp.empty())
                    {
                        return -1;
                    }
                    positive = tmp;
                }
            }


            {
                std::set<std::string> tmp;
                if (do_check_rsingle_diff_ge(url_para_map, i, tmp, positive, SETS_OP_INTERSECTION))
                {
                    if (tmp.empty())
                    {
                        return -1;
                    }
                    positive = tmp;
                }
            }


            {
                std::set<std::string> tmp;
                if (do_check_hsingle_sum_diff_ge(url_para_map, i, tmp, positive, SETS_OP_INTERSECTION))
                {
                    if (tmp.empty())
                    {
                        return -1;
                    }
                    positive = tmp;
                }
            }

            {
                std::set<std::string> tmp;
                if (do_check_hsingle_diff_ge(url_para_map, i, tmp, positive, SETS_OP_INTERSECTION))
                {
                    if (tmp.empty())
                    {
                        return -1;
                    }
                    positive = tmp;
                }
            }

        }

    }

    //去除停牌的
    {
        for (auto ii = p_data->_block_set.current()->begin(); ii != p_data->_block_set.current()->end(); ii++)
        {
            negative.insert(*ii); 
        }
    }

    std::set_difference(positive.begin(), positive.end(), negative.begin(), negative.end(), std::inserter(res,res.begin()));

    return 0;
}

int skhttp_res_data_process::url_select(std::map<std::string, std::string> & url_para_map, Value & root, Document::AllocatorType & allocator)
{
    std::set<std::string> res;

    do_check_select(url_para_map, res);
    std::map<std::string, std::string> tmp_map;

    for (auto ii = res.begin(); ii != res.end(); ii++)
    {
        tmp_map.clear();
        tmp_map["id"] = *ii;

        url_query_id(tmp_map, root, allocator);
    }

    return HTPP_RES_OK;
}

void skhttp_res_data_process::msg_recv_finish()
{
    http_req_head_para & req_head_para = _base_process->get_req_head_para();

    std::shared_ptr<base_net_obj> net_obj = get_base_net();
    net_addr & peer_addr = net_obj->get_peer_addr();

    LOG_NOTICE("peer ip[%s] peer port[%d] url_path[%s]", peer_addr.ip.c_str(), peer_addr.port, req_head_para._url_path.c_str());

    std::map<std::string, std::string> url_para_map;

    Document document;
    Document::AllocatorType& allocator = document.GetAllocator(); 
    Value root(kObjectType); 
    Value data_array(kArrayType);

    int recode;
    if (!strncmp(req_head_para._url_path.c_str(), "/queryid", strlen("/queryid"))){
        parse_url_para(req_head_para._url_path, url_para_map);
        recode = url_query_id(url_para_map, data_array, allocator);
    }else if (!strncmp(req_head_para._url_path.c_str(), "/select", strlen("/select"))){
        do_parse_request(url_para_map);
        recode = url_select(url_para_map, data_array, allocator);
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

    LOG_NOTICE("response:%s", buffer.GetString());
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
    //res_head._headers.insert(std::make_pair("Connection", "keep-alive"));
    res_head._headers.insert(std::make_pair("Connection", "close"));
    snprintf(proc_name, sizeof(proc_name), "%d", _body.length());
    res_head._headers.insert(std::make_pair("Content-Length", proc_name));
    res_head.to_head_str(str);

    LOG_DEBUG("%s", str->c_str());
    return str;
}

int skhttp_res_data_process::do_parse_request(std::map<std::string, std::string> & url_para_map)
{
    if (_recv_buf.empty())
        return -1;

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

