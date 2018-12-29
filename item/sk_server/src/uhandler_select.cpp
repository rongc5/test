#include "uhandler_select.h"
#include "sk_util.h"
#include "stringbuffer.h"
#include "writer.h"
#include "document.h"
#include "memorystream.h"
#include "prettywriter.h"
#include "rapidjson.h"
#include <algorithm>
#include "uhandler_queryid.h"

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

using namespace rapidjson;

void uhandler_select::perform(http_req_head_para * req_head, std::string * recv_body, http_res_head_para * res_head, std::string * send_body)
{
    if (!req_head || !recv_body || !send_body || !res_head)
        return;

    char t_buf[SIZE_LEN_256];
    t_buf[0] = '\0';
    proc_data* p_data = proc_data::instance();

    int recode = HTPP_RES_OK;
    Document document;
    Document::AllocatorType& allocator = document.GetAllocator(); 
    Value root(kObjectType); 
    Value data_array(kArrayType);

    std::set<std::string> res;
    std::map<std::string, std::string> url_para_map;

    do_parse_request(recv_body, url_para_map);

    if (!has_key<std::string, std::string>(url_para_map, "block") && !has_key<std::string, std::string>(url_para_map, "block_v"))
        {
            url_para_map["block_v"] = "1";    
        }

    do_check_select(url_para_map, res);
    std::map<std::string, std::string> tmp_map;

    for (auto ii = res.begin(); ii != res.end(); ii++)
    {
        tmp_map.clear();
        tmp_map["id"] = *ii;

        uhandler_queryid::do_check_queryid(tmp_map, data_array, allocator);
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

    send_body->append(buffer.GetString());

    res_head->_headers.insert(std::make_pair("Date", SecToHttpTime(time(NULL))));
    res_head->_headers.insert(std::make_pair("Server", p_data->proc_name));
    res_head->_headers.insert(std::make_pair("Connection", "close"));

    snprintf(t_buf, sizeof(t_buf), "%d", send_body->length());
    res_head->_headers.insert(std::make_pair("Content-Length", t_buf));
}

int uhandler_select::do_parse_request(std::string * _recv_buf, std::map<std::string, std::string> & url_para_map)
{
    if (!_recv_buf || _recv_buf->empty())
        return -1;

    std::vector<std::string> items;
    SplitString(_recv_buf->c_str(), "\r\n", &items, SPLIT_MODE_ALL);

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

int uhandler_select::do_check_select(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res)
{
    proc_data* p_data = proc_data::instance();
    if (!p_data)
        return -1;

    std::map<std::string, std::string>::iterator it;
    std::shared_ptr<base_search_index>  search_index;
    std::set<std::string> positive, negative;

    for (it = url_para_map.begin(); it != url_para_map.end(); it++)
    {
        std::string key = it->first;
        std::string value = it->second;

        search_index = p_data->get_search_index(key);
        if (!search_index)
            continue;

        std::set<std::string> tmp;
        search_index->search(key, value, tmp);
        
        if (end_with(key, "_v"))
        {
            if (tmp.empty())
                continue;

            if (negative.empty())
            {
                negative = tmp;
            }
            else
            {
                std::vector<std::set<std::string> >  arr;
                arr.push_back(negative);
                arr.push_back(tmp);

                get_union(arr, negative);
            }
        }else
        {
            if (tmp.empty())
            {
                return -1;
            }
            
            if (positive.empty())
            {
                positive = tmp;
            }
            else 
            {
                std::vector<std::set<std::string> >  arr;
                arr.push_back(positive);
                arr.push_back(tmp);

                get_intersection(arr, positive);
            }

        }
    }

    std::set_difference(positive.begin(), positive.end(), negative.begin(), negative.end(), std::inserter(res,res.begin()));

    return 0;
}

