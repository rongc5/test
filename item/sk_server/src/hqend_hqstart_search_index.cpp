#include "hqend_hqstart_search_index.h"

bool hqend_hqstart_search_index::search(std::string &key, std::string &value, std::set<std::string> & search)
{
   if (!key.compare("hqend_hqstart_ge") || !key.compare("hqend_hqstart_ge_v"))
   {
       return do_check_hqend_hqstart_ge(key, value, search);
   }
   else if (!key.compare("hqend_hqstart_le") || !key.compare("hqend_hqstart_le_v"))
   {
       return do_check_hqend_hqstart_le(key, value, search);
   }
   else if (!key.compare("hqstart_ge_num_ge") || !key.compare("hqstart_ge_num_ge_v"))
   {
       return do_check_hqend_hqstart_ge_num_ge(key, value, search);
   }

   return false;
}

bool hqend_hqstart_search_index::do_check_hqend_hqstart_le(std::string &key, std::string &value, std::set<std::string> & search)
{
    float end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it;
    std::multimap<float, std::string> * search_index = current();
    SETS_OP_TRPE tmp_ot;

    std::vector<std::string> tmp_vec;

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
        tmp_vec.push_back();

    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        std::string date;
        std::set<std::string> t_res; 
        search_index->get_last_date(atoi(t_vec[0].c_str()), date);
        if (date.empty()) 
        { 
            tmp_res_vec.push_back(t_res);
            continue;
        }

        end = atoi(t_vec[1].c_str());
        auto ii = search_index->find(date); 
        if (ii == search_index->end())
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

bool hqend_hqstart_search_index::do_check_hqend_hqstart_ge(std::string &key, std::string &value, std::set<std::string> & search)
{
    float end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it;
    std::map<std::string, std::multimap<float, std::string> >  * search_index = current();
    SETS_OP_TRPE tmp_ot;

    std::vector<std::string> tmp_vec;

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
        tmp_vec.push_back();

    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        std::string date;
        std::set<std::string> t_res; 
        search_index->get_last_date(atoi(t_vec[0].c_str()), date);
        if (date.empty()) 
        { 
            tmp_res_vec.push_back(t_res);
            continue;
        }

        end = atof(t_vec[1].c_str());
        auto ii = search_index->find(date); 
        if (ii == search_index->end())
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

bool hqend_hqstart_search_index::do_check_hqend_hqstart_ge_num_ge(std::string &key, std::string &value, std::set<std::string> & search)
{
    float end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it;
    std::map<std::string, std::multimap<float, std::string> >  * search_index = current();
    SETS_OP_TRPE tmp_ot;

    std::vector<std::string> tmp_vec;

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
        tmp_vec.push_back();

    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        std::string date;
        std::map<std::string, int> t_res;
        std::set<std::string> t_vv;
        search_index->get_last_date(atoi(t_vec[0].c_str()), date);
        if (date.empty()) 
        { 
            tmp_res_vec.push_back(t_res);
            continue;
        }

        end = atof(t_vec[1].c_str());
        auto ii = search_index->find(date); 
        if (ii == search_index->end())
        {
            tmp_res_vec.push_back(t_res);
            continue;;
        }

        for (; ii != search_index->end(); ii++)
        {
            it_le = ii->second.end();
            it_ge = ii->second.begin();

            it_ge = ii->second.lower_bound(end);
            for (it = it_ge; it != it_le; ++it)
            {
                std::map<std::string, int>::iterator itm = t_res.find(it->second);
                if (itm == t_res.end())
                {
                    t_res[it->second] = 1;
                }
                else
                {
                    t_res[it->second]++;
                }
            }
        }

        std::map<std::string, int>::iterator itm;
        for (itm = t_res.begin(); itm != t_res.end(); itm++)
        {
            if (itm->second >= atoi(t_vec[2].c_str()))
                t_vv.insert(itm->first);
        } 

        tmp_res_vec.push_back(t_vv);
    }

    if (tmp_ot == SETS_OP_INTERSECTION)
        get_intersection(tmp_res_vec, search);
    else
        get_union(tmp_res_vec, search);

    return true;
}
