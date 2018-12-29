#include "hsingle_diff_search_index.h"
#include "sk_util.h"
#include "proc_data.h"
#include "history_single_dict.h"

bool hsingle_diff_search_index::search(std::string &key, std::string &value, std::set<std::string> & search)
{
   if (start_with(key, "hsingle_diff_0_ge") || start_with(key, "hsingle_diff_1_ge") || start_with(key, "hsingle_diff_2_ge"))
   {
       return do_check_hsingle_diff_ge(key, value, search);
   }
   else if (start_with(key, "hsingle_diff_0_le") || start_with(key, "hsingle_diff_1_le") || start_with(key, "hsingle_diff_2_le"))
   {
       return do_check_hsingle_diff_le(key, value, search);
   }

   return false;
}

bool hsingle_diff_search_index::do_check_hsingle_diff_le(std::string &key, std::string &value, std::set<std::string> & search)
{
    uint32_t index = 0;
    int end = 0;
    std::multimap<int, std::string>::iterator it_le, it_ge, it;
    std::vector<std::map<std::string, std::multimap<int, std::string> > > * search_index = current();
    SETS_OP_TRPE tmp_ot;
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<std::set<std::string> > tmp_res_vec;
    SplitString(key.c_str(), '_', &tmp_vec, SPLIT_MODE_ALL);
    if (tmp_vec.size() < 4)
        return false;

    index = atoi(tmp_vec[2].c_str());

    if (index >= search_index->size()) 
        return false;

    if (strstr(key.c_str(), "|")) 
    {
        SplitString(value.c_str(), '|', &tmp_vec, SPLIT_MODE_ALL);
        tmp_ot = SETS_OP_UNION;
    }
    else 
    {
        SplitString(value.c_str(), '&', &tmp_vec, SPLIT_MODE_ALL); 
        tmp_ot = SETS_OP_INTERSECTION;
    }

    if (!tmp_vec.size())
        tmp_vec.push_back(value);

    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        std::string date;
        std::set<std::string> t_res; 
        p_data->_hsingle_dict->current()->get_last_date(atoi(t_vec[0].c_str()), date);
        if (date.empty()) 
        { 
            tmp_res_vec.push_back(t_res);
            continue;
        }

        end = atoi(t_vec[1].c_str());
        auto ii = search_index->at(index).find(date); 
        if (ii == search_index->at(index).end())
        {
            tmp_res_vec.push_back(t_res);
            continue;;
        }

        it_le = ii->second.end();
        it_ge = ii->second.begin();

        it_le = ii->second.upper_bound(end);

        for (it = it_ge; it != it_le; ++it)
        {
            t_res.insert(it->second);
        }

        tmp_res_vec.push_back(t_res);
    }

    if (tmp_ot == SETS_OP_INTERSECTION)
        get_intersection(tmp_res_vec, search);
    else
        get_union(tmp_res_vec, search);

    return true;
}

bool hsingle_diff_search_index::do_check_hsingle_diff_ge(std::string &key, std::string &value, std::set<std::string> & search)
{
    uint32_t index = 0;
    int end = 0;
    std::multimap<int, std::string>::iterator it_le, it_ge, it;
    std::vector<std::map<std::string, std::multimap<int, std::string> > > * search_index = current();
    SETS_OP_TRPE tmp_ot;
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<std::set<std::string> > tmp_res_vec;
    SplitString(key.c_str(), '_', &tmp_vec, SPLIT_MODE_ALL);
    if (tmp_vec.size() < 4)
        return false;

    index = atoi(tmp_vec[2].c_str());

    if (index >= search_index->size()) 
        return false;

    if (strstr(key.c_str(), "|")) 
    {
        SplitString(value.c_str(), '|', &tmp_vec, SPLIT_MODE_ALL);
        tmp_ot = SETS_OP_UNION;
    }
    else 
    {
        SplitString(value.c_str(), '&', &tmp_vec, SPLIT_MODE_ALL); 
        tmp_ot = SETS_OP_INTERSECTION;
    }

    if (!tmp_vec.size())
        tmp_vec.push_back(value);

    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        std::string date;
        std::set<std::string> t_res; 
        p_data->_hsingle_dict->current()->get_last_date(atoi(t_vec[0].c_str()), date);
        if (date.empty()) 
        { 
            tmp_res_vec.push_back(t_res);
            continue;
        }

        end = atoi(t_vec[1].c_str());
        auto ii = search_index->at(index).find(date); 
        if (ii == search_index->at(index).end())
        {
            tmp_res_vec.push_back(t_res);
            continue;;
        }

        it_le = ii->second.end();
        it_ge = ii->second.begin();

        it_ge = ii->second.lower_bound(end);

        for (it = it_ge; it != it_le; ++it)
        {
            t_res.insert(it->second);
        }

        tmp_res_vec.push_back(t_res);
    }

    if (tmp_ot == SETS_OP_INTERSECTION)
        get_intersection(tmp_res_vec, search);
    else
        get_union(tmp_res_vec, search);

    return true;

}



