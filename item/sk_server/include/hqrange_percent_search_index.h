#ifndef __hqrange_percent_SEARCH_INDEX_H__
#define __hqrange_percent_SEARCH_INDEX_H__

#include "sk_def.h"
#include "common_util.h"

class hqrange_percent_search_index:public base_search_index, public ToBufferMgr<std::map<std::string, std::multimap<float, std::string> > >
{
    public:
        virtual ~hqrange_percent_search_index(){}

        virtual bool search(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hqrange_percent_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hqrange_percent_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hqrange_percent_ge_num_ge(std::string &key, std::string &value, std::set<std::string> & search);

};


#endif

