#include "rsingle_search_index.h"

bool rsingle_search_index::do_check_rsingle_le(std::string &key, std::string &value, std::set<std::string> & search)
{
    uint32_t index = 0;
    int end = 0;
    std::multimap<int, std::string>::iterator it_le, it_ge, it;

    auto & search_index = current()->rsingle_diff_index;


    std::vector<std::string> tmp_vec;
    SplitString(key.c_str(), '_', &tmp_vec, SPLIT_MODE_ALL);
    if (tmp_vec.size() < 4)
        return false;

    index = atoi(tmp_vec[2].c_str());

    if (index >= search_index.size()) 
        return false;

    it_le = search_index[index].end();
    it_ge = search_index[index].begin();
    end = atoi(value.c_str());
    it_le = search_index[index].upper_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {
        search.insert(it->second);
    }

    return true;
}

bool rsingle_search_index::do_check_rsingle_ge(std::string &key, std::string &value, std::set<std::string> & search)
{
    uint32_t index = 0;
    int end = 0;
    std::multimap<int, std::string>::iterator it_le, it_ge, it;

    auto & search_index = current()->rsingle_diff_index;

    std::vector<std::string> tmp_vec;
    SplitString(key.c_str(), '_', &tmp_vec, SPLIT_MODE_ALL);
    if (tmp_vec.size() < 4)
        return false;

    index = atoi(tmp_vec[2].c_str());

    if (index >= search_index.size()) 
        return false;

    it_le = search_index[index].end();
    it_ge = search_index[index].begin();
    end = atoi(value.c_str());
    it_ge = search_index[index].lower_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {
        search.insert(it->second);
    }

    return true;
}

bool rsingle_search_index::do_check_rsingle_diff2_le(std::string &key, std::string &value, std::set<std::string> & search)
{
    uint32_t index = 0;
    int end = 0;
    std::multimap<int, std::string>::iterator it_le, it_ge, it;

    auto & search_index = current()->rsingle_diff2_index;


    std::vector<std::string> tmp_vec;
    SplitString(key.c_str(), '_', &tmp_vec, SPLIT_MODE_ALL);
    if (tmp_vec.size() < 4)
        return false;

    index = atoi(tmp_vec[2].c_str());

    if (index >= search_index.size()) 
        return false;

    it_le = search_index[index].end();
    it_ge = search_index[index].begin();

    end = atoi(value.c_str());
    it_le = search_index[index].upper_bound(end);


    for (it = it_ge; it != it_le; ++it)
    {
        search.insert(it->second);
    }

    return true;
}

bool rsingle_search_index::do_check_rsingle_diff2_ge(std::string &key, std::string &value, std::set<std::string> & search)
{
    uint32_t index = 0;
    int end = 0;
    std::multimap<int, std::string>::iterator it_le, it_ge, it;

    auto & search_index = current()->rsingle_diff2_index;

    std::vector<std::string> tmp_vec;
    SplitString(key.c_str(), '_', &tmp_vec, SPLIT_MODE_ALL);
    if (tmp_vec.size() < 4)
        return false;

    index = atoi(tmp_vec[2].c_str());

    if (index >= search_index.size())
        return false;

    it_le = search_index[index].end();
    it_ge = search_index[index].begin();
    end = atoi(value.c_str());
    it_ge = search_index[index].lower_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {
        search.insert(it->second);
    }

    return true;
}


