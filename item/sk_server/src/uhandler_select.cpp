#include "uhandler_select.h"
#include "sk_util.h"

void perform(http_req_head_para * req_head, std::string * recv_body, http_res_head_para * res_head, std::string * send_body)
{
	
	
	Document document;
   Document::AllocatorType& allocator = document.GetAllocator(); 
   Value root(kObjectType); 
   Value data_array(kArrayType);
	
	std::set<std::string> res;
	std::map<std::string, std::string> url_para_map;
	
	do_parse_request(recv_body, url_para_map);
	
   do_check_select(url_para_map, res);
   std::map<std::string, std::string> tmp_map;

    for (auto ii = res.begin(); ii != res.end(); ii++)
    {
        tmp_map.clear();
        tmp_map["id"] = *ii;

        url_query_id(tmp_map, root, allocator);
    }
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

bool uhandler_select::do_check_hsingle_diff_ge(std::map<std::string, std::string> & url_para_map, uint32_t index, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_hsingle_sum_diff_ge(std::map<std::string, std::string> & url_para_map, uint32_t index, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
{
    int end = 0;
    bool flag = false;
    proc_data* p_data = proc_data::instance();
    char t_buf[SIZE_LEN_128]; 

    std::vector<std::string> tmp_vec;
    std::vector<std::set<std::string> > tmp_res_vec;

    std::multimap<int, std::string>::iterator it_le, it_ge, it;
    if (index >= p_data->_hsingle_sum_diff_index.current()->size())
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

bool uhandler_select::do_check_rsingle_diff_ge(std::map<std::string, std::string> & url_para_map, uint32_t index, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_rsingle_diff2_ge(std::map<std::string, std::string> & url_para_map, uint32_t index, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_plate(std::map<std::string, std::string> & url_para_map, const char * key, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_address(std::map<std::string, std::string> & url_para_map, const char * key, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_jlrgr_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_jlrgr_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_id_substr(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)  
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

bool uhandler_select::do_check_yylrgr_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_yylrgr_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_zysrgr_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_end_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_end_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_change_rate_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_change_rate_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_range_percent_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_history_sum_range_percent_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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


bool uhandler_select::do_check_history_has_range_percent_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_history_range_percent_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_range_percent_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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


bool uhandler_select::do_check_history_sum_range_percent_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_end_hqend_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_history_has_range_percent_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_history_range_percent_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_down_pointer_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_down_pointer_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_up_pointer_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_up_pointer_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_end_avg_price_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_end_avg_price_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_pe_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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


bool uhandler_select::do_check_pe_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_pb_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_pb_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_value_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_value_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_cir_value_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_cir_value_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_cir_value_num_min(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_cir_value_num_max(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_value_num_min(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_value_num_max(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res,  std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_mgxj_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_mgxj_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_mgsy_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_mgsy_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_mgsygr_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_mgsygr_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_mgxjgr_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_mgxjgr_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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

bool uhandler_select::do_check_zysrgr_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et)
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
