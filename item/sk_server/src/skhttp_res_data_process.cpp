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

#include "sk_util.h"
#include "rsingle_data_process.h"
#include "rquotation_data_process.h"

#include "uhandler_select.h"


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

void skhttp_res_data_process::query_sum_quotation(uint32_t last_day_num, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();
    std::string date;

    auto ii = p_data->_hquoation_dict->current()->_id_date_dict.find(id);
    if (ii == p_data->_hquoation_dict->current()->_id_date_dict.end())
    {
        return;
    }

    uint32_t i = 0;
    for (auto iii = ii->second.rbegin(); iii != ii->second.rend() && i < last_day_num; i++, iii++)
    {
        date = *iii;
    }

    query_sum_quotation(date, id, root, allocator);
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

void skhttp_res_data_process::query_sum_quotation(std::string & history_date, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();
    char t_buf[SIZE_LEN_64];

    quotation_t qt;
    bool flag = rquotation_data_process::get_sum_quotation(id, history_date, qt);
    if (flag)
    {
        Value k(kStringType);
        Value v(kObjectType);

        Value key(kStringType);
        Value value(kStringType);

        std::string t_str;
        t_str.append("sum_quotation");
        t_str.append("_");
        t_str.append(history_date);

        k.SetString(t_str.c_str(), allocator);

        {
            key.SetString("range_percent", allocator); 
            value.SetString(float_2_str(qt.range_percent).c_str(), allocator); 

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
        query_sum_quotation(atoi(url_para_map["history_num"].c_str()), url_para_map["id"], root, allocator);
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
        query_sum_quotation(url_para_map["history_date"], url_para_map["id"], root, allocator);
    }

    data_array.PushBack(root, allocator);

    return HTPP_RES_OK;
}


void skhttp_res_data_process::msg_recv_finish()
{
    http_req_head_para & req_head_para = _base_process->get_req_head_para();

    std::shared_ptr<base_net_obj> net_obj = get_base_net();
    net_addr & peer_addr = net_obj->get_peer_addr();

    LOG_NOTICE("peer ip[%s] peer port[%d] url_path[%s]", peer_addr.ip.c_str(), peer_addr.port, req_head_para._url_path.c_str());

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

    sa_process->reg_handler("/select?", std::make_shared<uhandler_select>());
    sa_process->reg_handler("/queryid?", std::make_shared<uhandler_select>());
}

void reg_handler(std::string & url, std::shared_ptr<url_handler> & handler)
{
    _uhandler_map[url] = handler;
}

