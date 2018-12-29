#ifndef __mgxjgr_SEARCH_INDEX_H__
#define __mgxjgr_SEARCH_INDEX_H__

#include "sk_def.h"
#include "common_util.h"

class mgxjgr_search_index:public base_search_index, public ToBufferMgr<std::multimap<float, std::string> >
{
    public:
        virtual ~mgxjgr_search_index(){}

        virtual bool search(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_mgxjgr_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_mgxjgr_ge(std::string &key, std::string &value, std::set<std::string> & search);
};


#endif

