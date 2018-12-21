#include "plate_search_index.h"

bool plate_search_index::search(std::string &key, std::string &value, std::set<std::string> & search)
{
    std::vector<std::string> tmp_vec;
    SplitString(value.c_str(), '|', &tmp_vec, SPLIT_MODE_ALL);
    if (!tmp_vec.size()) 
        tmp_vec.push_back(value);

    std::unordered_multimap<std::shared_ptr<std::string>, std::string, str_hasher, str_equaler> * search_index = current();
    for (uint32_t i = 0; i< tmp_vec.size(); i++) 
    {
        std::shared_ptr<std::string> ss(new std::string(trim(tmp_vec[i].c_str())));
        auto range = search_index->equal_range(ss);
        for (auto it = range.first; it != range.second; ++it)
        {
                search.insert(it->second);
        }   
    }


   return true;
}


