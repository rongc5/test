#include "up_pointer_search_index.h"

bool up_pointer_search_index::search(std::string &key, std::string &value, std::set<std::string> & search)
{
   if (!key.compare("up_pointer_ge") || !key.compare("up_pointer_ge_v"))
   {
       return do_check_up_pointer_ge(key, value, search);
   }
   else if (!key.compare("up_pointer_le") || !key.compare("up_pointer_le_v"))
   {
       return do_check_up_pointer_le(key, value, search);
   }

   return false;
}

bool up_pointer_search_index::do_check_up_pointer_le(std::string &key, std::string &value, std::set<std::string> & search)
{
    float end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it;

    std::multimap<float, std::string> * search_index = current();

    it_le = search_index->end();
    it_ge = search_index->begin();

    end = atof(value.c_str());
    it_le = search_index->upper_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {
        search.insert(it->second);
    }

    return true;
}

bool up_pointer_search_index::do_check_up_pointer_ge(std::string &key, std::string &value, std::set<std::string> & search)
{
    float end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it;

    std::multimap<float, std::string> * search_index = current();
    it_le = search_index->end();
    it_ge = search_index->begin();

    end = atof(value.c_str());
    it_ge = search_index->lower_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {
        search.insert(it->second);
    }

    return true;
}



