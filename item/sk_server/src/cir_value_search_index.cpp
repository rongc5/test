#include "cir_value_search_index.h"

bool cir_value_search_index::search(std::string &key, std::string &value, std::set<std::string> & search)
{
   if (!key.compare("cir_value_ge") || !key.compare("cir_value_ge_v"))
   {
       return do_check_cir_value_ge(key, value, search);
   }
   else if (!key.compare("cir_value_le") || !key.compare("cir_value_le_v"))
   {
       return do_check_cir_value_le(key, value, search);
   }

   return false;
}

bool cir_value_search_index::do_check_cir_value_le(std::string &key, std::string &value, std::set<std::string> & search)
{
    int end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it;

    std::multimap<float, std::string> * search_index = current();

    it_le = search_index->end();
    it_ge = search_index->begin();

    end = atoi(value.c_str());
    it_le = search_index->upcir_valuer_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {
        search.insert(it->second);
    }

    return true;
}

bool cir_value_search_index::do_check_cir_value_ge(std::string &key, std::string &value, std::set<std::string> & search)
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


