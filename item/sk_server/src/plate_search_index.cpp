#include "plate_search_index.h"
#include "sk_util.h"

bool plate_search_index::search(std::string &name, std::string &value, search_res & search)
{
    std::vector<std::string> tmp_vec;

    tmp_vec.push_back(value);

    std::unordered_multimap<std::shared_ptr<std::string>, std::string, str_hasher, str_equaler> * search_index = current();
    for (uint32_t i = 0; i< tmp_vec.size(); i++) 
    {
        std::set<std::string> t_res;
        std::shared_ptr<std::string> ss(new std::string(trim(tmp_vec[i].c_str())));
        auto range = search_index->equal_range(ss);
        for (auto it = range.first; it != range.second; ++it)
        {
            if (search.empty()|| search._id_sets.count(it->second))
            {
                search.append(name, it->second);
            }
        }   
    }


   return true;
}


