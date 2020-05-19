#include "plate_search_index.h"
#include "sk_util.h"

bool plate_search_index::search(std::string &key, std::string &value, search_res & search)
{
    SETS_OP_TRPE tmp_ot;
    std::vector<std::string> tmp_vec;
    std::vector<std::set<std::string> > tmp_res_vec;
    
    if (strstr(value.c_str(), "|")) 
    {
        SplitString(value.c_str(), '|', &tmp_vec, SPLIT_MODE_ALL);
        tmp_ot = SETS_OP_UNION;
    }
    else
    {
        SplitString(value.c_str(), ';', &tmp_vec, SPLIT_MODE_ALL); 
        tmp_ot = SETS_OP_INTERSECTION;
    }

    if (!tmp_vec.size()) 
        tmp_vec.push_back(value);

    std::unordered_multimap<std::shared_ptr<std::string>, std::string, str_hasher, str_equaler> * search_index = current();
    for (uint32_t i = 0; i< tmp_vec.size(); i++) 
    {
        std::set<std::string> t_res;
        std::shared_ptr<std::string> ss(new std::string(trim(tmp_vec[i].c_str())));
        auto range = search_index->equal_range(ss);
        for (auto it = range.first; it != range.second; ++it)
        {
                t_res.insert(it->second);
        }   

        tmp_res_vec.push_back(t_res);
    }

    if (tmp_ot == SETS_OP_INTERSECTION)
        get_intersection(tmp_res_vec, search._id_sets);
    else
        get_union(tmp_res_vec, search._id_sets);


   return true;
}


