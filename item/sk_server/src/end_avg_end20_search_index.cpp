#include "end_avg_end20_search_index.h"

bool end_avg_end20_search_index::search(std::string &key, std::string &value, std::set<std::string> & search)
{
   if (!key.compare("end_avg_end20_ge") || !key.compare("end_avg_end20_ge_v"))
   {
       return do_check_end_avg_end20_ge(key, value, search);
   }
   else if (!key.compare("end_avg_end20_le") || !key.compare("end_avg_end20_le_v"))
   {
       return do_check_end_avg_end20_le(key, value, search);
   }

   return false;
}

bool end_avg_end20_search_index::do_check_end_avg_end20_le(std::string &key, std::string &value, std::set<std::string> & search)
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

bool end_avg_end20_search_index::do_check_end_avg_end20_ge(std::string &key, std::string &value, std::set<std::string> & search)
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



