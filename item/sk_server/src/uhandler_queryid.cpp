#include "uhandler_queryid.h"
#include "proc_data.h"
#include "sk_util.h"
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

void uhandler_queryid::perform(http_req_head_para * req_head, std::string * recv_body, http_res_head_para * res_head, std::string * send_body)
{
    if (!req_head || !recv_body || !send_body || !res_head)
        return;

    proc_data* p_data = proc_data::instance();
    int recode;
    char t_buf[SIZE_LEN_256];
    t_buf[0] = '\0';

    Document document;
    Document::AllocatorType& allocator = document.GetAllocator(); 
    Value root(kObjectType); 
    Value data_array(kArrayType);

    std::map<std::string, std::string> url_para_map;

    parse_url_para(req_head->_url_path, url_para_map);
    recode = do_check_queryid(url_para_map, data_array, allocator);

    Value key(kStringType);    
    Value value(kStringType); 

    key.SetString("recode", allocator);
    root.AddMember(key, recode, allocator);

    key.SetString("data", allocator);

    root.AddMember(key, data_array, allocator);

    StringBuffer buffer;    
    Writer<StringBuffer> writer(buffer);    
    root.Accept(writer);

    send_body->append(buffer.GetString());

    res_head->_headers.insert(std::make_pair("Date", SecToHttpTime(time(NULL))));
    res_head->_headers.insert(std::make_pair("Server", p_data->proc_name));
    res_head->_headers.insert(std::make_pair("Connection", "close"));

    snprintf(t_buf, sizeof(t_buf), "%d", send_body->length());
    res_head->_headers.insert(std::make_pair("Content-Length", t_buf));

}


int uhandler_queryid::do_check_queryid(std::map<std::string, std::string> & url_para_map, Value & data_array, Document::AllocatorType & allocator)
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

    {
        Value child(kObjectType);
        query_technical(url_para_map["id"], child, allocator);

        key.SetString("technical", allocator);
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

void uhandler_queryid::query_quotation(std::string &id, Value & root, Document::AllocatorType & allocator)
{
    Value key(kStringType);
    Value value(kStringType);

    proc_data* p_data = proc_data::instance();
    std::unordered_map<std::string, std::shared_ptr<quotation_t>, str_hasher> * rquoation_dict_index = NULL;
    rquoation_dict_index = p_data->_rquoation_dict_index.current();
    auto ii = rquoation_dict_index->find(id);
    if (ii != rquoation_dict_index->end())
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
    }
}

void uhandler_queryid::query_technical(std::string &id, Value & root, Document::AllocatorType & allocator)
{
    Value key(kStringType);
    Value value(kStringType);

    proc_data* p_data = proc_data::instance();
    std::unordered_map<std::string, std::shared_ptr<technical_t>, str_hasher> * rtechnical_dict_index = NULL;
    rtechnical_dict_index = p_data->_rtechnical_dict_index.current();
    auto ii = rtechnical_dict_index->find(id);
    if (ii != rtechnical_dict_index->end())
    {
        {
            key.SetString("avg_end", allocator); 
            value.SetString(float_2_str(ii->second->avg_end).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("end_5", allocator); 
            value.SetString(float_2_str(ii->second->end_5).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("end_10", allocator); 
            value.SetString(float_2_str(ii->second->end_10).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("end_20", allocator); 
            value.SetString(float_2_str(ii->second->end_20).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("end_30", allocator); 
            value.SetString(float_2_str(ii->second->end_30).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("avg_end_5", allocator); 
            value.SetString(float_2_str(ii->second->avg_end_5).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("avg_end_10", allocator); 
            value.SetString(float_2_str(ii->second->avg_end_10).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("avg_end_20", allocator); 
            value.SetString(float_2_str(ii->second->avg_end_20).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("avg_end_30", allocator); 
            value.SetString(float_2_str(ii->second->avg_end_30).c_str(), allocator); 

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
    }
}

void uhandler_queryid::query_plate(std::string &id, Value & root, Document::AllocatorType & allocator)
{
    Value value(kStringType);
    proc_data* p_data = proc_data::instance();
    plate_dict * _plate_dict = p_data->_plate_dict->current();
    auto ii = _plate_dict->_id_dict.find(id);
    if (ii != _plate_dict->_id_dict.end())
    {
        for (auto ft: ii->second)
        {
            value.SetString(ft->c_str(), allocator); 
            root.PushBack(value, allocator);
        }
    }
}

void uhandler_queryid::query_single(std::string &id, Value & root, Document::AllocatorType & allocator)
{
    char t_buf[SIZE_LEN_64];
    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current();
    std::unordered_map<std::string, std::deque<std::shared_ptr<single_vec> >, str_hasher> * rsingle_dict_index = NULL;
    rsingle_dict_index = p_data->_rsingle_dict_index.current();

    auto ii = rsingle_dict_index->find(id);
    if (ii != rsingle_dict_index->end())
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

void uhandler_queryid::query_single_in(std::string &id, Value & root, Document::AllocatorType & allocator)
{
    char t_buf[SIZE_LEN_64];
    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current();
    std::unordered_map<std::string, std::deque<std::shared_ptr<single_vec> >, str_hasher> * rsingle_dict_index = NULL;
    rsingle_dict_index = p_data->_rsingle_dict_index.current();

    auto ii = rsingle_dict_index->find(id);

    if (ii != rsingle_dict_index->end())
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

void uhandler_queryid::query_single_out(std::string &id, Value & root, Document::AllocatorType & allocator)
{
    char t_buf[SIZE_LEN_64];
    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current();
    std::unordered_map<std::string, std::deque<std::shared_ptr<single_vec> >, str_hasher> * rsingle_dict_index = NULL;
    rsingle_dict_index = p_data->_rsingle_dict_index.current();

    auto ii = rsingle_dict_index->find(id);

    if (ii != rsingle_dict_index->end())
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

void uhandler_queryid::query_blocked(std::string &id, Value & root, Document::AllocatorType & allocator)
{
    Value key(kStringType);
    Value value(kStringType);

    key.SetString("block", allocator);
    proc_data* p_data = proc_data::instance();
    if (p_data->_block_set->do_check_block(id))
    {
        root.AddMember(key, 1, allocator);
    }
    else
    {
        root.AddMember(key, 0, allocator);
    }
}

void uhandler_queryid::query_addr(std::string &id, Value & root, Document::AllocatorType & allocator)
{
    Value key(kStringType);
    Value value(kStringType);

    key.SetString("addr", allocator);
    proc_data* p_data = proc_data::instance();
    addr_dict * _addr_dict = p_data->_addr_dict->current();
    auto ii = _addr_dict->_id_dict.find(id);
    if (ii != _addr_dict->_id_dict.end())
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


void uhandler_queryid::query_finance(std::string &id, Value & root, Document::AllocatorType & allocator)
{
    Value key(kStringType);
    Value value(kStringType);

    proc_data* p_data = proc_data::instance();
    std::unordered_map<std::string, std::shared_ptr<finance_t>, str_hasher> * finance_dict_index;
    finance_dict_index = p_data->_finance_dict_index.current();
    auto ii = finance_dict_index->find(id);
    if (ii != finance_dict_index->end())
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
            key.SetString("zysrgr", allocator); 
            value.SetString(float_2_str(ii->second->zysrgr).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("jlrgr", allocator); 
            value.SetString(float_2_str(ii->second->jlrgr).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }
    }
}

void uhandler_queryid::query_history_single(uint32_t last_day_num, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();
    char t_buf[SIZE_LEN_64];

    std::vector<std::string> date_vec;

    history_single_dict * _hsingle_dict = p_data->_hsingle_dict->current();
    {
        auto ii = _hsingle_dict->_id_date_dict.find(id);
        if (ii == _hsingle_dict->_id_date_dict.end())
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

        auto ii = _hsingle_dict->_date_dict.find(key);
        if (ii != _hsingle_dict->_date_dict.end())
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

void uhandler_queryid::query_sum_single(uint32_t last_day_num, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();

    std::string date;

    history_single_dict * _hsingle_dict = p_data->_hsingle_dict->current();
    auto ii = _hsingle_dict->_id_date_dict.find(id);
    if (ii == _hsingle_dict->_id_date_dict.end())
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

void uhandler_queryid::query_history_single_in(uint32_t last_day_num, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();
    char t_buf[SIZE_LEN_64];

    std::vector<std::string> date_vec;

    {
        history_single_dict * _hsingle_dict = p_data->_hsingle_dict->current();
        auto ii = _hsingle_dict->_id_date_dict.find(id);
        if (ii == _hsingle_dict->_id_date_dict.end())
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

        history_single_dict * _hsingle_dict = p_data->_hsingle_dict->current();
        auto ii = _hsingle_dict->_date_dict.find(key);
        if (ii != _hsingle_dict->_date_dict.end())
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

void uhandler_queryid::query_history_single_out(uint32_t last_day_num, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();
    char t_buf[SIZE_LEN_64];

    std::vector<std::string> date_vec;

    {
        history_single_dict * _hsingle_dict = p_data->_hsingle_dict->current();
        auto ii = _hsingle_dict->_id_date_dict.find(id);
        if (ii == _hsingle_dict->_id_date_dict.end())
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

        history_single_dict * _hsingle_dict = p_data->_hsingle_dict->current();
        auto ii = _hsingle_dict->_date_dict.find(key);
        if (ii != _hsingle_dict->_date_dict.end())
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

void uhandler_queryid::query_history_single(std::string & history_date, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();

    std::string key;    
    history_single_dict::creat_key(history_date, id, key);

    history_single_dict * _hsingle_dict = p_data->_hsingle_dict->current();
    auto ii = _hsingle_dict->_date_dict.find(key);
    if (ii != _hsingle_dict->_date_dict.end())
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

void uhandler_queryid::query_sum_single(std::string & history_date, std::string &id, Value & root, Document::AllocatorType & allocator)
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

void uhandler_queryid::query_history_single_in(std::string & history_date, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();

    std::string key;    
    history_single_dict::creat_key(history_date, id, key);

    history_single_dict * _hsingle_dict = p_data->_hsingle_dict->current();
    auto ii = _hsingle_dict->_date_dict.find(key);
    if (ii != _hsingle_dict->_date_dict.end())
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

void uhandler_queryid::query_history_single_out(std::string & history_date, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();

    std::string key;    
    history_single_dict::creat_key(history_date, id, key);

    history_single_dict * _hsingle_dict = p_data->_hsingle_dict->current();
    auto ii = _hsingle_dict->_date_dict.find(key);
    if (ii != _hsingle_dict->_date_dict.end())
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

void uhandler_queryid::query_history_quotation(uint32_t last_day_num, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();
    char t_buf[SIZE_LEN_64];

    std::vector<std::string> date_vec;

    {
        history_quotation_dict * _hquoation_dict = p_data->_hquoation_dict->current();
        auto ii = _hquoation_dict->_id_date_dict.find(id);
        if (ii == _hquoation_dict->_id_date_dict.end())
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

        history_quotation_dict * _hquoation_dict = p_data->_hquoation_dict->current();
        auto ii = _hquoation_dict->_id_dict.find(key);
        if (ii != _hquoation_dict->_id_dict.end())
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

void uhandler_queryid::query_sum_quotation(uint32_t last_day_num, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();
    std::string date;

    history_quotation_dict * _hquoation_dict = p_data->_hquoation_dict->current();
    auto ii = _hquoation_dict->_id_date_dict.find(id);
    if (ii == _hquoation_dict->_id_date_dict.end())
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

void uhandler_queryid::query_history_quotation(std::string & history_date, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();
    char t_buf[SIZE_LEN_64];


    std::string key;    
    history_quotation_dict::creat_key(history_date, id, key);

    history_quotation_dict * _hquoation_dict = p_data->_hquoation_dict->current();
    auto ii = _hquoation_dict->_id_dict.find(key);
    if (ii != _hquoation_dict->_id_dict.end())
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

void uhandler_queryid::query_sum_quotation(std::string & history_date, std::string &id, Value & root, Document::AllocatorType & allocator)
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
