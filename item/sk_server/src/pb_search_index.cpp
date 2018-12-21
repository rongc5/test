#include "pb_search_index.h"

bool pb_search_index::search(std::string &key, std::string &value, std::set<std::string> & search)
{
   if (!key.compare("pb_ge") || !key.compare("pb_ge_v"))
   {
       return do_check_pb_ge(key, value, search);
   }
   else if (!key.compare("pb_le") || !key.compare("pb_le_v"))
   {
       return do_check_pb_le(key, value, search);
   }

   return false;
}

bool pb_search_index::do_check_pb_le(std::string &key, std::string &value, std::set<std::string> & search)
{
    int end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it;

    std::multimap<float, std::string> * search_index = current();

    it_le = search_index->end();
    it_ge = search_index->begin();

    end = atoi(value.c_str());
    it_le = search_index->uppbr_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {
        search.insert(it->second);
    }

    return true;
}

bool pb_search_index::do_check_pb_ge(std::string &key, std::string &value, std::set<std::string> & search)
{
    int end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it;

    std::multimap<float, std::string> * search_index = current();
    it_le = search_index->end();
    it_ge = search_index->begin();

    end = atoi(value.c_str());
    it_ge = search_index->lower_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {
        search.insert(it->second);
    }

    return true;
}



