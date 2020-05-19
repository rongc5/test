#include "block_search_index.h"
#include "sk_util.h"
#include "proc_data.h"

bool block_search_index::search(std::string &key, std::string &value, search_res & search)
{
   if (!key.compare("block") || !key.compare("block_v"))
   {
       return do_check_block_search(key, value, search);
   }

   return false;
}

bool block_search_index::do_check_block_search(std::string &key, std::string &value, search_res & search)
{
    std::unordered_set<std::string, str_hasher>::iterator it_le, it_ge, it;

    std::unordered_set<std::string, str_hasher> * search_index = current();

    it_le = search_index->end();
    it_ge = search_index->begin();

    for (it = it_ge; it != it_le; ++it)
    {
        search._id_sets.insert(*it);
    }

    return true;
}

bool block_search_index::do_check_block(const std::string &key)
{
    std::unordered_set<std::string, str_hasher>::iterator it;
    std::unordered_set<std::string, str_hasher> * search_index = current();

    it = search_index->find(key);
    if (it != search_index->end())
    {
        return true;
    }

    return false;
}



