#include "rsingle_diff2_search_index.h"

bool rsingle_diff2_search_index::search(std::string &key, std::string &value, std::set<std::string> & search)
{
   if (start_with(key, "rsingle_diff2_0_ge") || start_with(key, "rsingle_diff2_1_ge") || start_with(key, "rsingle_diff2_2_ge"))
   {
       return do_check_rsingle_diff2_ge(key, value, search);
   }
   else if (start_with(key, "rsingle_diff2_0_le") || start_with(key, "rsingle_diff2_1_le") || start_with(key, "rsingle_diff2_2_le"))
   {
       return do_check_rsingle_diff2_le(key, value, search);
   }

   return false;
}

bool rsingle_diff2_search_index::do_check_rsingle_diff2_le(std::string &key, std::string &value, std::set<std::string> & search)
{
    uint32_t index = 0;
    int end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it;

    std::multimap<float, std::string> * search_index = current();

    it_le = search_index->end();
    it_ge = search_index->begin();

    std::vector<std::string> tmp_vec;
    SplitString(ptr, '_', &tmp_vec, SPLIT_MODE_ALL);
    if (tmp_vec.size() < 4)
        return false;

    index = atoi(tmp_vec[2].c_str());

    if (index >= search_index->size()) 
        return false;

    end = atoi(value.c_str());
    it_le = search_index->uprsingle_diff2r_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {
        search.insert(it->second);
    }

    return true;
}

bool rsingle_diff2_search_index::do_check_rsingle_diff2_ge(std::string &key, std::string &value, std::set<std::string> & search)
{
    uint32_t index = 0;
    int end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it;

    std::multimap<float, std::string> * search_index = current();
    it_le = search_index->end();
    it_ge = search_index->begin();

    std::vector<std::string> tmp_vec;
    SplitString(ptr, '_', &tmp_vec, SPLIT_MODE_ALL);
    if (tmp_vec.size() < 4)
        return false;

    index = atoi(tmp_vec[2].c_str());

    if (index >= search_index->size()) 
        return false;

    end = atoi(value.c_str());
    it_ge = search_index->lower_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {
        search.insert(it->second);
    }

    return true;
}



