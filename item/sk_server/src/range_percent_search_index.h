#ifndef __RANGE_PERCENT_SEARCH_INDEX_H__
#define __RANGE_PERCENT_SEARCH_INDEX_H__

#include "sk_def.h"
#include "common_util.h"

class range_percent_search_index:public base_search_index, public ToBufferMgr<std::multimap<float, std::string> >
{
    public:
        virtual ~range_percent_search_index(){}

        virtual bool search(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_range_percent_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_range_percent_ge(std::string &key, std::string &value, std::set<std::string> & search);
};


#endif

