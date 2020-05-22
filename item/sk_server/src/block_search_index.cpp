#include "block_search_index.h"
#include "sk_util.h"
#include "proc_data.h"


bool block_search_index::do_check_block_search(std::string &name, std::string &value, search_res & search)
{
    std::unordered_set<std::string, str_hasher>::iterator it_le, it_ge, it;

    std::unordered_set<std::string, str_hasher> * search_index = current();

    it_le = search_index->end();
    it_ge = search_index->begin();

    for (it = it_ge; it != it_le; ++it)
    {
        if (search.empty() || search._id_sets.count(*it))
        {
            search.append(name, *it);
        }

    }

    return true;
}

bool block_search_index::do_check_block(const std::string &name)
{
    std::unordered_set<std::string, str_hasher>::iterator it;
    std::unordered_set<std::string, str_hasher> * search_index = current();

    it = search_index->find(name);
    if (it != search_index->end())
    {
        return true;
    }

    return false;
}



