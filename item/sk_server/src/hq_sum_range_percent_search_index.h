#ifndef __hq_sum_range_percent_SEARCH_INDEX_H__
#define __hq_sum_range_percent_SEARCH_INDEX_H__

#include "sk_def.h"
#include "common_util.h"

class hq_sum_range_percent_search_index:public base_search_index, public ToBufferMgr<std::map<std::string, std::multimap<float, std::string> > >
{
    public:
        virtual ~hq_sum_range_percent_search_index(){}

        virtual bool search(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hq_sum_range_percent_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hq_sum_range_percent_ge(std::string &key, std::string &value, std::set<std::string> & search);
};


#endif

