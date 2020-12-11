#include "uhandler_queryid.h"
#include "proc_data.h"
#include "sk_util.h"
#include "finance_dict.h"
#include "plate_dict.h"
#include "cir_holder_dict.h"
#include "addr_dict.h"
#include "history_single_dict.h"
#include "history_quotation_dict.h"
#include "base_net_obj.h"
#include "id_dict.h"
#include "userid_dict.h"

#include "sk_util.h"
#include "rsingle_data_process.h"
#include "rquotation_data_process.h"
#include "hsingle_search_index.h"

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

    std::string decode_path;
    UrlDecode(req_head->_url_path, decode_path);
    parse_url_para(decode_path, url_para_map);
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
        if (!has_key<std::string, std::string>(url_para_map, "id"))
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

    if (has_key<std::string, std::string>(url_para_map, "cir_holder"))
    {
        //Value child(kArrayType);
        query_cir_holder(url_para_map["id"], root, allocator);

        //key.SetString("cir_holder", allocator);
        //root.AddMember(key, child, allocator);
    }

    {
        Value child(kObjectType);
        query_single(url_para_map["id"], child, allocator);

        key.SetString("single", allocator);
        root.AddMember(key, child, allocator);
    }

    {
        Value child(kObjectType);
        query_single_ratio(url_para_map["id"], child, allocator);

        key.SetString("sratio", allocator);
        root.AddMember(key, child, allocator);
    }

    {
        Value child(kObjectType);
        query_single_vratio(url_para_map["id"], child, allocator);

        key.SetString("svr", allocator);
        root.AddMember(key, child, allocator);
    }


    {
        Value child(kObjectType);
        query_technical(url_para_map["id"], child, allocator);

        key.SetString("technical", allocator);
        root.AddMember(key, child, allocator);
    }

    /*
    {
        query_blocked(url_para_map["id"], root, allocator);
    }
    */

    {
        query_addr(url_para_map["id"], root, allocator);
    }

    if (has_key<std::string, std::string>(url_para_map, "history_num"))
    {
        query_history_single(atoi(url_para_map["history_num"].c_str()), url_para_map["id"], root, allocator);

        query_sum_single(atoi(url_para_map["history_num"].c_str()), url_para_map["id"], root, allocator);

        bool has_userid = has_key<std::string, std::string>(url_para_map, "userid");

        if (has_userid && p_data->_userid_dict->is_userid_valid(url_para_map["userid"], *p_data->get_req_date()))
        {
            query_history_single_ratio(atoi(url_para_map["history_num"].c_str()), url_para_map["id"], root, allocator);
            query_sum_single_ratio(atoi(url_para_map["history_num"].c_str()), url_para_map["id"], root, allocator);

            query_history_single_vratio(atoi(url_para_map["history_num"].c_str()), url_para_map["id"], root, allocator);
            query_sum_single_vratio(atoi(url_para_map["history_num"].c_str()), url_para_map["id"], root, allocator);

        }

        query_history_quotation(atoi(url_para_map["history_num"].c_str()), url_para_map["id"], root, allocator);
        query_sum_quotation(atoi(url_para_map["history_num"].c_str()), url_para_map["id"], root, allocator);
    }

    if (has_key<std::string, std::string>(url_para_map, "history_wnum"))
    {
        query_history_wsingle(atoi(url_para_map["history_wnum"].c_str()), url_para_map["id"], root, allocator);

        query_sum_wsingle(atoi(url_para_map["history_wnum"].c_str()), url_para_map["id"], root, allocator);

        query_history_wquotation(atoi(url_para_map["history_wnum"].c_str()), url_para_map["id"], root, allocator);
        query_sum_wquotation(atoi(url_para_map["history_wnum"].c_str()), url_para_map["id"], root, allocator);
    }

    if (has_key<std::string, std::string>(url_para_map, "history_date"))
    {
        query_history_single(url_para_map["history_date"], url_para_map["id"], root, allocator);

        query_sum_single(url_para_map["history_date"], url_para_map["id"], root, allocator);

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
    auto & rquoation_dict_index = p_data->_rquotation_index->current()->id_quotation;
    auto ii = rquoation_dict_index.find(id);
    if (ii != rquoation_dict_index.end())
    {
        {
            key.SetString("name", allocator); 
            value.SetString(ii->second.back()->name, allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("start", allocator); 
            value.SetString(float_2_str(ii->second.back()->start).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("end", allocator); 
            value.SetString(float_2_str(ii->second.back()->end).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("high", allocator); 
            value.SetString(float_2_str(ii->second.back()->high).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("low", allocator); 
            value.SetString(float_2_str(ii->second.back()->low).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("lclosed", allocator); 
            value.SetString(float_2_str(ii->second.back()->last_closed).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("crate", allocator); 
            value.SetString(float_2_str(ii->second.back()->change_rate).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("rpercent", allocator); 
            value.SetString(float_2_str(ii->second.back()->range_percent).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("qr_ratio", allocator); 
            value.SetString(float_2_str(ii->second.back()->quantity_relative_ratio).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }
    }
}

void uhandler_queryid::query_technical(std::string &id, Value & root, Document::AllocatorType & allocator)
{
    Value key(kStringType);
    Value value(kStringType);

    proc_data* p_data = proc_data::instance();
    auto & rtechnical_dict_index = p_data->_rquotation_index->current()->id_technical;
    auto ii = rtechnical_dict_index.find(id);
    if (ii != rtechnical_dict_index.end())
    {
        {
            key.SetString("avg_end", allocator); 
            value.SetString(float_2_str(ii->second.back()->avg_end).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("end_5", allocator); 
            value.SetString(float_2_str(ii->second.back()->end_5).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("end_10", allocator); 
            value.SetString(float_2_str(ii->second.back()->end_10).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("end_20", allocator); 
            value.SetString(float_2_str(ii->second.back()->end_20).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        /*
        {
            key.SetString("end_30", allocator); 
            value.SetString(float_2_str(ii->second.back()->end_30).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }
        */

        {
            key.SetString("down_pointer", allocator); 
            value.SetString(float_2_str(ii->second.back()->down_pointer).c_str(), allocator); 

            root.AddMember(key, value, allocator);
        }

        {
            key.SetString("up_pointer", allocator); 
            value.SetString(float_2_str(ii->second.back()->up_pointer).c_str(), allocator); 

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

void uhandler_queryid::query_cir_holder(std::string &id, Value & root, Document::AllocatorType & allocator)
{
    int count = 0;
    proc_data* p_data = proc_data::instance();
    cir_holder_dict * _cir_holder_dict = p_data->_cir_holder_dict->current();
    auto ii = _cir_holder_dict->_id_dict.find(id);
    if (ii != _cir_holder_dict->_id_dict.end())
    {
        for (auto ft: ii->second)
        {
            count++;

            std::string key =  "cir_holder_" + std::to_string(count);
            Value k(kStringType);
            Value v(kStringType);

            k.SetString(key.c_str(), allocator);

            v.SetString(ft->c_str(), allocator); 

            root.AddMember(k, v, allocator);
        }
    }
}

void uhandler_queryid::query_single(std::string &id, Value & root, Document::AllocatorType & allocator)
{
    char t_buf[SIZE_LEN_64];
    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current();
    auto rsingle_dict_index = p_data->_rsingle_index->current();

    auto ii = rsingle_dict_index->id_single.find(id);
    if (ii != rsingle_dict_index->id_single.end())
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

void uhandler_queryid::query_single_ratio(std::string &id, Value & root, Document::AllocatorType & allocator)
{
    char t_buf[SIZE_LEN_64];
    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current();
    auto rsingle_dict_index = p_data->_rsingle_index->current();

    auto ii = rsingle_dict_index->id_single.find(id);
    if (ii != rsingle_dict_index->id_single.end())
    {
        for (uint32_t i = 0; i < strategy->real_single_scale.size(); i++)
        {
            Value key(kStringType);
            Value child(kArrayType);

            snprintf(t_buf, sizeof(t_buf), "sratio_%d", i);
            key.SetString(t_buf, allocator);

            for (auto ft: ii->second)
            {
                if (i < ft->size())
                {
                    //float dratio = (ft->at(i).in + ft->at(i).out)? ft->at(i).diff*1.0/(ft->at(i).in + ft->at(i).out):0;
                    std::string str = float_2_str(ft->at(i).dratio * 100).append("%");
                    child.PushBack(Value().SetString(str.c_str(), str.length(), allocator), allocator);
                }
            }
            root.AddMember(key, child, allocator);
        }
    }
}


void uhandler_queryid::query_single_vratio(std::string &id, Value & root, Document::AllocatorType & allocator)
{
    char t_buf[SIZE_LEN_64];
    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current();
    auto rsingle_dict_index = p_data->_rsingle_index->current();

    int vol  = 0;
    auto & rquoation_dict_index = p_data->_rquotation_index->current()->id_quotation;
    auto qq = rquoation_dict_index.find(id);
    if (qq != rquoation_dict_index.end())
    {
        vol = qq->second.back()->vol;
    }

    auto ii = rsingle_dict_index->id_single.find(id);
    if (ii != rsingle_dict_index->id_single.end())
    {
        for (uint32_t i = 0; i < strategy->real_single_scale.size(); i++)
        {
            Value key(kStringType);
            Value child(kArrayType);

            snprintf(t_buf, sizeof(t_buf), "svr_%d", i);
            key.SetString(t_buf, allocator);

            for (auto ft: ii->second)
            {
                if (i < ft->size())
                {
                    float dratio = vol ? ft->at(i).diff*1.0/vol:0;
                    std::string str = float_2_str(dratio * 100).append("%");
                    child.PushBack(Value().SetString(str.c_str(), str.length(), allocator), allocator);
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
    finance_search_item * finance_dict_index = p_data->_finance_index->current();
    auto ii = finance_dict_index->id_finance.find(id);
    if (ii != finance_dict_index->id_finance.end())
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

    hsingle_search_item * _hsingle_dict = p_data->_hsingle_index->current();

    auto ii = _hsingle_dict->id_single.find(id);
    if (ii == _hsingle_dict->id_single.end())
    {
        return;
    }

    int len = 0;
    for (uint32_t i = 0; i< last_day_num && i < ii->second.size(); i++)
    {
        len = ii->second.size() - i - 1;
        if (len < 0)
            continue;

        std::string key;           
        hsingle_search_item::creat_id_index_key(id, len, key);

        auto mm = _hsingle_dict->id_idx_date.find(key);
        if (mm != _hsingle_dict->id_idx_date.end())
        {
            Value k(kStringType);
            Value child(kObjectType);

            std::string t_str;
            t_str.append("single");
            t_str.append("_");
            t_str.append(mm->second);

            k.SetString(t_str.c_str(), allocator);

            const std::deque<std::shared_ptr<single_vec>> & dq = ii->second[len];

            for (uint32_t k = 0; !dq.empty() && k < dq[0]->size(); k++)
            {
                Value key_1(kStringType);
                Value child_1(kArrayType);

                snprintf(t_buf, sizeof(t_buf), "vol_%d", k);
                key_1.SetString(t_buf, allocator);

                for (uint32_t j = 0; j < dq.size(); j++)
                {
                    child_1.PushBack(dq[j]->at(k).diff, allocator);
                }

                child.AddMember(key_1, child_1, allocator);
            }

            root.AddMember(k, child, allocator);
        }
    }
}

void uhandler_queryid::query_history_single_ratio(uint32_t last_day_num, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();
    char t_buf[SIZE_LEN_64];

    hsingle_search_item * _hsingle_dict = p_data->_hsingle_index->current();

    auto ii = _hsingle_dict->id_single.find(id);
    if (ii == _hsingle_dict->id_single.end())
    {
        return;
    }

    int len = 0;
    for (uint32_t i = 0; i< last_day_num && i < ii->second.size(); i++)
    {
        len = ii->second.size() - i - 1;
        if (len < 0)
            continue;

        std::string key;           
        hsingle_search_item::creat_id_index_key(id, len, key);

        auto mm = _hsingle_dict->id_idx_date.find(key);
        if (mm != _hsingle_dict->id_idx_date.end())
        {
            Value k(kStringType);
            Value child(kObjectType);

            std::string t_str;
            t_str.append("sratio");
            t_str.append("_");
            t_str.append(mm->second);

            k.SetString(t_str.c_str(), allocator);

            const std::deque<std::shared_ptr<single_vec>> & dq = ii->second[len];

            for (uint32_t k = 0; !dq.empty() && k < dq[0]->size(); k++)
            {
                Value key_1(kStringType);
                Value child_1(kArrayType);

                snprintf(t_buf, sizeof(t_buf), "sratio_%d", k);
                key_1.SetString(t_buf, allocator);

                for (uint32_t j = 0; j < dq.size(); j++)
                {
                    //float dratio = (dq[j]->at(k).in + dq[j]->at(k).out)? dq[j]->at(k).diff*1.0/(dq[j]->at(k).in + dq[j]->at(k).out):0;
                    std::string str = float_2_str( dq[j]->at(k).dratio * 100).append("%");
                    child_1.PushBack(Value().SetString(str.c_str(), str.length(), allocator), allocator);
                }

                child.AddMember(key_1, child_1, allocator);
            }

            root.AddMember(k, child, allocator);
        }
    }
}


void uhandler_queryid::query_history_single_vratio(uint32_t last_day_num, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();
    char t_buf[SIZE_LEN_64];

    hsingle_search_item * _hsingle_dict = p_data->_hsingle_index->current();

    auto ii = _hsingle_dict->id_single.find(id);
    if (ii == _hsingle_dict->id_single.end())
    {
        return;
    }

    hquotation_search_item * hqitem = p_data->_hquotation_index->current();
    auto ff = hqitem->id_quotation.find(id);
    if (ff == hqitem->id_quotation.end())
    {    
        return;
    }

    int len = 0;
    int vlen = 0;
    std::string date;
    for (uint32_t i = 0; i< last_day_num && i < ii->second.size(); i++)
    {
        len = ii->second.size() - i - 1;
        if (len < 0)
            continue;

          if (i > ff->second.size())
            continue;

        
        vlen  = ff->second.size() - i;
        date = hqitem->get_date(id, vlen);
        int vol =0;
        if (!date.empty())
            vol = ff->second[vlen]->vol;

        std::string key;           
        hsingle_search_item::creat_id_index_key(id, len, key);

        auto mm = _hsingle_dict->id_idx_date.find(key);
        if (mm != _hsingle_dict->id_idx_date.end())
        {
            Value k(kStringType);
            Value child(kObjectType);

            std::string t_str;
            t_str.append("svr");
            t_str.append("_");
            t_str.append(mm->second);

            k.SetString(t_str.c_str(), allocator);

            const std::deque<std::shared_ptr<single_vec>> & dq = ii->second[len];

            for (uint32_t k = 0; !dq.empty() && k < dq[0]->size(); k++)
            {
                Value key_1(kStringType);
                Value child_1(kArrayType);

                snprintf(t_buf, sizeof(t_buf), "svr_%d", k);
                key_1.SetString(t_buf, allocator);

                for (uint32_t j = 0; j < dq.size(); j++)
                {
                    float dratio = (vol)? dq[j]->at(k).diff*1.0/vol:0;
                    std::string str = float_2_str(dratio * 100).append("%");
                    child_1.PushBack(Value().SetString(str.c_str(), str.length(), allocator), allocator);
                }

                child.AddMember(key_1, child_1, allocator);
            }

            root.AddMember(k, child, allocator);
        }
    }
}


void uhandler_queryid::query_history_wsingle(uint32_t last_day_num, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();
    char t_buf[SIZE_LEN_64];

    hsingle_search_item * _hsingle_dict = p_data->_hwsingle_index->current();

    auto ii = _hsingle_dict->id_single.find(id);
    if (ii == _hsingle_dict->id_single.end())
    {
        return;
    }

    std::set<std::string>  date;

    int len = 0;
    for (uint32_t i = 0; i< last_day_num && i < ii->second.size(); i++)
    {
        len = ii->second.size() - i - 1;
        if (len < 0)
            continue;

        std::string key;           
        hsingle_search_item::creat_id_index_key(id, len, key);

        auto mm = _hsingle_dict->id_idx_date.find(key);
        if (mm != _hsingle_dict->id_idx_date.end())
        {
            Value k(kStringType);
            Value child(kObjectType);

            std::string t_str;
            t_str.append("wsingle");
            t_str.append("_");
            t_str.append(mm->second);

            p_data->_wtdate_set->get_trade_date(mm->second, date);

            if (!date.empty())
            {    
                t_str += "_" + *date.begin() + "_" + *date.rbegin();
            } 


            k.SetString(t_str.c_str(), allocator);

            const std::deque<std::shared_ptr<single_vec>> & dq = ii->second[len];

            for (uint32_t k = 0; !dq.empty() && k < dq[0]->size(); k++)
            {
                Value key_1(kStringType);
                Value child_1(kArrayType);

                snprintf(t_buf, sizeof(t_buf), "vol_%d", k);
                key_1.SetString(t_buf, allocator);

                for (uint32_t j = 0; j < dq.size(); j++)
                {
                    child_1.PushBack(dq[j]->at(k).diff, allocator);
                }

                child.AddMember(key_1, child_1, allocator);
            }

            root.AddMember(k, child, allocator);
        }
    }
}

void uhandler_queryid::query_sum_wsingle(uint32_t last_day_num, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();

    std::string date;

    hsingle_search_item * hsitem = p_data->_hwsingle_index->current();
    auto ii = hsitem->id_sum_single.find(id);
    if (ii == hsitem->id_sum_single.end())
    {
        return;
    }

    int i = 0;
    int len = 0;
    for ( i = last_day_num;  i >= 0; i--)
    {
        if (i > (int)ii->second.size() - 1)
            continue;
        
        len  = ii->second.size() - i -1;
        date = hsitem->get_date(id, len);
        if (!date.empty()) {
            query_sum_wsingle(date, id, root, allocator);
            break;
        }
    }

}

void uhandler_queryid::query_sum_single(uint32_t last_day_num, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();

    std::string date;

    hsingle_search_item * hsitem = p_data->_hsingle_index->current();
    auto ii = hsitem->id_sum_single.find(id);
    if (ii == hsitem->id_sum_single.end())
    {
        return;
    }

    int i = 0;
    int len = 0;
    for ( i = last_day_num;  i >= 0; i--)
    {
        if (i > (int)ii->second.size() - 1)
            continue;
        
        len  = ii->second.size() - i -1;
        date = hsitem->get_date(id, len);
        if (!date.empty()) {
            query_sum_single(date, id, root, allocator);
            break;
        }
    }

}

void uhandler_queryid::query_sum_single_ratio(uint32_t last_day_num, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();

    std::string date;

    hsingle_search_item * hsitem = p_data->_hsingle_index->current();
    auto ii = hsitem->id_sum_single.find(id);
    if (ii == hsitem->id_sum_single.end())
    {
        return;
    }

    int i = 0;
    int len = 0;
    for ( i = last_day_num;  i >= 0; i--)
    {
        if (i > (int)ii->second.size() - 1)
            continue;
        
        len  = ii->second.size() - i -1;
        date = hsitem->get_date(id, len);
        if (!date.empty()) {
            query_sum_single_ratio(date, id, root, allocator);
            break;
        }
    }

}


void uhandler_queryid::query_sum_single_vratio(uint32_t last_day_num, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();

    std::string date;

    hsingle_search_item * hsitem = p_data->_hsingle_index->current();
    auto ii = hsitem->id_sum_single.find(id);
    if (ii == hsitem->id_sum_single.end())
    {
        return;
    }

    int i = 0;
    int len = 0;
    for ( i = last_day_num;  i >= 0; i--)
    {
        if (i > (int)ii->second.size() - 1)
            continue;
        
        len  = ii->second.size() - i -1;
        date = hsitem->get_date(id, len);
        if (!date.empty()) {
            query_sum_single_vratio(date, id, root, allocator);
            break;
        }
    }

}


void uhandler_queryid::query_history_wsingle(std::string & history_date, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();
    char t_buf[SIZE_LEN_64];  

    hsingle_search_item * hsitem = p_data->_hwsingle_index->current();
    auto ii = hsitem->id_single.find(id);
    if (ii == hsitem->id_single.end())
    {    
        return;
    }
    std::set<std::string>  date;

    int index = hsitem->get_index(id, history_date);
    if (index >= 0)
    {
        Value k(kStringType);
        Value child(kObjectType);

        std::string t_str;
        t_str.append("wsingle");
        t_str.append("_");
        t_str.append(history_date);

        p_data->_wtdate_set->get_trade_date(history_date, date);

        if (!date.empty())
        {    
            t_str += "_" + *date.begin() + "_" + *date.rbegin();
        } 

        k.SetString(t_str.c_str(), allocator);

        for (uint32_t k = 0; k < ii->second[index].back()->size(); k++)
        {
            Value key_1(kStringType);
            Value child_1(kArrayType);

            snprintf(t_buf, sizeof(t_buf), "vol_%d", k);
            key_1.SetString(t_buf, allocator);

            for (uint32_t j = 0; j < ii->second[index].size(); j++)
            {
                child_1.PushBack(ii->second[index][j]->at(k).diff, allocator);
            }

            child.AddMember(key_1, child_1, allocator);
        }

        root.AddMember(k, child, allocator);
    }
}

void uhandler_queryid::query_history_single(std::string & history_date, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();
    char t_buf[SIZE_LEN_64];  

    hsingle_search_item * hsitem = p_data->_hsingle_index->current();
    auto ii = hsitem->id_single.find(id);
    if (ii == hsitem->id_single.end())
    {    
        return;
    }
    int index = hsitem->get_index(id, history_date);
    if (index >= 0)
    {
        Value k(kStringType);
        Value child(kObjectType);

        std::string t_str;
        t_str.append("single");
        t_str.append("_");
        t_str.append(history_date);

        k.SetString(t_str.c_str(), allocator);

        for (uint32_t k = 0; k < ii->second[index].back()->size(); k++)
        {
            Value key_1(kStringType);
            Value child_1(kArrayType);

            snprintf(t_buf, sizeof(t_buf), "vol_%d", k);
            key_1.SetString(t_buf, allocator);

            for (uint32_t j = 0; j < ii->second[index].size(); j++)
            {
                child_1.PushBack(ii->second[index][j]->at(k).diff, allocator);
            }

            child.AddMember(key_1, child_1, allocator);
        }

        root.AddMember(k, child, allocator);
    }
}

void uhandler_queryid::query_sum_wsingle(std::string & history_date, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();

    single_vec st;
    std::string key;
    hsingle_search_item * hsitem = p_data->_hwsingle_index->current();
    
    int index = hsitem->get_index(id, history_date);
    auto ii = hsitem->id_sum_single.find(id);

    if (ii != hsitem->id_sum_single.end() && index >= 0)
    {
        Value k(kStringType);
        Value child(kArrayType);

        std::string t_str;
        t_str.append("wsingle_sum");
        t_str.append("_");
        t_str.append(history_date);

        k.SetString(t_str.c_str(), allocator);

        for (int k = ii->second[index]->size() - 1;k >= 0; k--)
        {
            child.PushBack(ii->second[ii->second.size() - 1]->at(k).diff - ii->second[index]->at(k).diff, allocator);
        }

        root.AddMember(k, child, allocator);
    }
}

void uhandler_queryid::query_sum_single(std::string & history_date, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();

    single_vec st;
    std::string key;
    hsingle_search_item * hsitem = p_data->_hsingle_index->current();
    
    int index = hsitem->get_index(id, history_date);
    auto ii = hsitem->id_sum_single.find(id);

    if (ii != hsitem->id_sum_single.end() && index >= 0)
    {
        Value k(kStringType);
        Value child(kArrayType);

        std::string t_str;
        t_str.append("single_sum");
        t_str.append("_");
        t_str.append(history_date);

        k.SetString(t_str.c_str(), allocator);

        for (int k = ii->second[index]->size() - 1;k >= 0; k--)
        {
            child.PushBack(ii->second[ii->second.size() - 1]->at(k).diff - ii->second[index]->at(k).diff, allocator);
        }

        root.AddMember(k, child, allocator);
    }
}

void uhandler_queryid::query_sum_single_ratio(std::string & history_date, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();

    single_vec st;
    std::string key;
    hsingle_search_item * hsitem = p_data->_hsingle_index->current();
    
    int index = hsitem->get_index(id, history_date);
    auto ii = hsitem->id_sum_single.find(id);

    if (ii != hsitem->id_sum_single.end() && index >= 0)
    {
        Value k(kStringType);
        Value child(kArrayType);

        std::string t_str;
        t_str.append("sratio_sum");
        t_str.append("_");
        t_str.append(history_date);

        k.SetString(t_str.c_str(), allocator);

        for (int k = ii->second[index]->size() - 1;k >= 0; k--)
        {
            unsigned long long sum = ii->second[ii->second.size() - 1]->at(k).in + ii->second[ii->second.size() - 1]->at(k).out;
            unsigned long long dsum = ii->second[index]->at(k).in + ii->second[index]->at(k).out;
            sum = dsum && sum? sum - dsum: 0;
            int diff = ii->second[ii->second.size() - 1]->at(k).diff - ii->second[index]->at(k).diff;
            float dratio = sum ? diff * 1.0 / sum : 0;
            std::string str = float_2_str(dratio * 100).append("%");
            child.PushBack(Value().SetString(str.c_str(), str.length(), allocator), allocator);
        }

        root.AddMember(k, child, allocator);
    }
}

void uhandler_queryid::query_sum_single_vratio(std::string & history_date, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();

    single_vec st;
    std::string key;
    hsingle_search_item * hsitem = p_data->_hsingle_index->current();
    
    int index = hsitem->get_index(id, history_date);
    auto ii = hsitem->id_sum_single.find(id);

    hquotation_search_item * hqitem = p_data->_hquotation_index->current();
    long vol = 0;
    int iindex = hqitem->get_index(id, history_date);
    auto ff = hqitem->id_sum_quotation.find(id); 
    if (ff != hqitem->id_sum_quotation.end() && iindex >= 1)
    {
        vol = ff->second[ii->second.size() - 1]->vol - ff->second[iindex]->vol;
    }

    if (ii != hsitem->id_sum_single.end() && index >= 0)
    {
        Value k(kStringType);
        Value child(kArrayType);

        std::string t_str;
        t_str.append("svr_sum");
        t_str.append("_");
        t_str.append(history_date);

        k.SetString(t_str.c_str(), allocator);

        for (int k = ii->second[index]->size() - 1;k >= 0; k--)
        {
            int diff = ii->second[ii->second.size() - 1]->at(k).diff - ii->second[index]->at(k).diff;
            float dratio = vol ? diff * 1.0 / vol : 0;
            std::string str = float_2_str(dratio * 100).append("%");
            child.PushBack(Value().SetString(str.c_str(), str.length(), allocator), allocator);
        }

        root.AddMember(k, child, allocator);
    }
}


void uhandler_queryid::query_history_wquotation(uint32_t last_day_num, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();
    char t_buf[SIZE_LEN_64];


    hquotation_search_item * hqitem = p_data->_hwquotation_index->current();
    auto ii = hqitem->id_quotation.find(id);
    if (ii == hqitem->id_quotation.end())
    {
        return;
    }

    int i = 0;
    int len = 0;
    std::string date;
    std::set<std::string>  date_set;
    for (i = last_day_num; i >= 0; i--)
    {
        if (i > (int)ii->second.size())
            continue;
        len  = ii->second.size() - i;

        date = hqitem->get_date(id, len);

        if (!date.empty())
        {
            Value k(kStringType);
            Value v(kObjectType);

            Value key(kStringType);
            Value value(kStringType);

            std::string t_str;
            t_str.append("wquotation");
            t_str.append("_");
            t_str.append(date);

            p_data->_wtdate_set->get_trade_date(date, date_set);
            if (!date_set.empty())
            {
                t_str += "_" + *date_set.begin() + "_" + *date_set.rbegin();
            }

            k.SetString(t_str.c_str(), allocator);

            {
                key.SetString("end", allocator); 
                value.SetString(float_2_str(ii->second[len]->end).c_str(), allocator); 

                v.AddMember(key, value, allocator);
            }

            {
                key.SetString("low", allocator); 
                value.SetString(float_2_str(ii->second[len]->low).c_str(), allocator); 

                v.AddMember(key, value, allocator);
            }

            {
                key.SetString("rpercent", allocator); 
                value.SetString(float_2_str(ii->second[len]->range_percent).c_str(), allocator); 

                v.AddMember(key, value, allocator);
            }
            {
                key.SetString("vol", allocator); 
                value.SetString(float_2_str(ii->second[len]->vol).c_str(), allocator); 

                v.AddMember(key, value, allocator);
            }


            root.AddMember(k, v, allocator);
        }
    }
}

void uhandler_queryid::query_history_quotation(uint32_t last_day_num, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();
    char t_buf[SIZE_LEN_64];


    hquotation_search_item * hqitem = p_data->_hquotation_index->current();
    auto ii = hqitem->id_quotation.find(id);
    if (ii == hqitem->id_quotation.end())
    {
        return;
    }

    int i = 0;
    int len = 0;
    std::string date;
    for (i = last_day_num; i >= 0; i--)
    {
        if (i > (int)ii->second.size())
            continue;
        len  = ii->second.size() - i;

        date = hqitem->get_date(id, len);

        if (!date.empty())
        {
            Value k(kStringType);
            Value v(kObjectType);

            Value key(kStringType);
            Value value(kStringType);

            std::string t_str;
            t_str.append("quotation");
            t_str.append("_");
            t_str.append(date);

            k.SetString(t_str.c_str(), allocator);

            {
                key.SetString("end", allocator); 
                value.SetString(float_2_str(ii->second[len]->end).c_str(), allocator); 

                v.AddMember(key, value, allocator);
            }

            {
                key.SetString("low", allocator); 
                value.SetString(float_2_str(ii->second[len]->low).c_str(), allocator); 

                v.AddMember(key, value, allocator);
            }

            {
                key.SetString("rpercent", allocator); 
                value.SetString(float_2_str(ii->second[len]->range_percent).c_str(), allocator); 

                v.AddMember(key, value, allocator);
            }

            {
                key.SetString("vol", allocator); 
                value.SetString(float_2_str(ii->second[len]->vol).c_str(), allocator); 

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

    hquotation_search_item * hqitem = p_data->_hquotation_index->current();
    auto ii = hqitem->id_sum_quotation.find(id);
    if (ii == hqitem->id_sum_quotation.end())
    {
        return;
    }  

    int i = 0;
    int len = 0;
    for ( i = last_day_num;  i >= 0; i--)
    {
        if (i > (int)ii->second.size() - 1)
            continue;

        len  = ii->second.size() - i - 1;
        date = hqitem->get_date(id, len);
        if (!date.empty()) {
            query_sum_quotation(date, id, root, allocator);
            break;
        }
    }
}

void uhandler_queryid::query_sum_wquotation(uint32_t last_day_num, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();
    std::string date;

    hquotation_search_item * hqitem = p_data->_hwquotation_index->current();
    auto ii = hqitem->id_sum_quotation.find(id);
    if (ii == hqitem->id_sum_quotation.end())
    {
        return;
    }  

    int i = 0;
    int len = 0;
    for ( i = last_day_num;  i >= 0; i--)
    {
        if (i > (int)ii->second.size() - 1)
            continue;

        len  = ii->second.size() - i - 1;
        date = hqitem->get_date(id, len);
        if (!date.empty()) {
            query_sum_wquotation(date, id, root, allocator);
            break;
        }
    }
}

void uhandler_queryid::query_history_wquotation(std::string & history_date, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();
    char t_buf[SIZE_LEN_64];


    hquotation_search_item * hqitem = p_data->_hwquotation_index->current();
    int index = hqitem->get_index(id, history_date);
    auto ii = hqitem->id_quotation.find(id);

    std::set<std::string>  date;

    p_data->_wtdate_set->get_trade_date(history_date, date);

    if (ii != hqitem->id_quotation.end() && index >= 0)
    {
        Value k(kStringType);
        Value v(kObjectType);

        Value key(kStringType);
        Value value(kStringType);

        std::string t_str;
        t_str.append("wquotation");
        t_str.append("_");
        t_str.append(history_date);
        if (!date.empty())
        {
            t_str += "_" + *date.begin() + "_" + *date.rbegin();
        }

        k.SetString(t_str.c_str(), allocator);

        {
            key.SetString("end", allocator); 
            value.SetString(float_2_str(ii->second[index]->end).c_str(), allocator); 

            v.AddMember(key, value, allocator);
        }

        {
            key.SetString("low", allocator); 
            value.SetString(float_2_str(ii->second[index]->low).c_str(), allocator); 

            v.AddMember(key, value, allocator);
        }

        {
            key.SetString("rpercent", allocator); 
            value.SetString(float_2_str(ii->second[index]->range_percent).c_str(), allocator); 

            v.AddMember(key, value, allocator);
        }

        {
            key.SetString("vol", allocator); 
            value.SetString(float_2_str(ii->second[index]->vol).c_str(), allocator); 

            v.AddMember(key, value, allocator);
        }

        root.AddMember(k, v, allocator);
    }
}

void uhandler_queryid::query_history_quotation(std::string & history_date, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();
    char t_buf[SIZE_LEN_64];


    hquotation_search_item * hqitem = p_data->_hquotation_index->current();
    int index = hqitem->get_index(id, history_date);
    auto ii = hqitem->id_quotation.find(id);

    if (ii != hqitem->id_quotation.end() && index >= 0)
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
            value.SetString(float_2_str(ii->second[index]->end).c_str(), allocator); 

            v.AddMember(key, value, allocator);
        }

        {
            key.SetString("low", allocator); 
            value.SetString(float_2_str(ii->second[index]->low).c_str(), allocator); 

            v.AddMember(key, value, allocator);
        }

        {
            key.SetString("rpercent", allocator); 
            value.SetString(float_2_str(ii->second[index]->range_percent).c_str(), allocator); 

            v.AddMember(key, value, allocator);
        }

        {
            key.SetString("vol", allocator); 
            value.SetString(float_2_str(ii->second[index]->vol).c_str(), allocator); 

            v.AddMember(key, value, allocator);
        }

        root.AddMember(k, v, allocator);
    }
}

void uhandler_queryid::query_sum_quotation(std::string & history_date, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();
    char t_buf[SIZE_LEN_64];

    hquotation_search_item * hqitem = p_data->_hquotation_index->current();
    int index = hqitem->get_index(id, history_date);
    auto ii = hqitem->id_sum_quotation.find(id);

    if (ii != hqitem->id_sum_quotation.end() && index >= 1)
    {
        Value k(kStringType);
        Value v(kObjectType);

        Value key(kStringType);
        Value value(kStringType);

        std::string t_str;
        t_str.append("quotation_sum");
        t_str.append("_");
        t_str.append(history_date);

        k.SetString(t_str.c_str(), allocator);

        {
            key.SetString("range_percent", allocator); 
            value.SetString(float_2_str(ii->second[ii->second.size() - 1]->range_percent - ii->second[index]->range_percent).c_str(), allocator); 

            v.AddMember(key, value, allocator);
        }

        root.AddMember(k, v, allocator);
    }
}

void uhandler_queryid::query_sum_wquotation(std::string & history_date, std::string &id, Value & root, Document::AllocatorType & allocator)
{
    proc_data* p_data = proc_data::instance();
    char t_buf[SIZE_LEN_64];

    hquotation_search_item * hqitem = p_data->_hwquotation_index->current();
    int index = hqitem->get_index(id, history_date);
    auto ii = hqitem->id_sum_quotation.find(id);

    if (ii != hqitem->id_sum_quotation.end() && index >= 1)
    {
        Value k(kStringType);
        Value v(kObjectType);

        Value key(kStringType);
        Value value(kStringType);

        std::string t_str;
        t_str.append("wquotation_sum");
        t_str.append("_");
        t_str.append(history_date);

        k.SetString(t_str.c_str(), allocator);

        {
            key.SetString("range_percent", allocator); 
            value.SetString(float_2_str(ii->second[ii->second.size() - 1]->range_percent - ii->second[index]->range_percent).c_str(), allocator); 

            v.AddMember(key, value, allocator);
        }

        root.AddMember(k, v, allocator);
    }
}

