#ifndef __hqchange_rate_SEARCH_INDEX_H__
#define __hqchange_rate_SEARCH_INDEX_H__

#include "sk_def.h"
#include "common_util.h"

class hqchange_rate_search_index:public base_search_index, public ToBufferMgr<std::map<std::string, std::multimap<float, std::string> > >
{
    public:
        virtual ~hqchange_rate_search_index(){}

        virtual bool search(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hqchange_rate_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hqchange_rate_ge(std::string &key, std::string &value, std::set<std::string> & search);
};


#endif

