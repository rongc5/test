#ifndef __mgxj_SEARCH_INDEX_H__
#define __mgxj_SEARCH_INDEX_H__

#include "sk_def.h"
#include "common_util.h"

class mgxj_search_index:public base_search_index, public ToBufferMgr<std::multimap<float, std::string> >
{
    public:
        virtual ~mgxj_search_index(){}

        virtual bool search(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_mgxj_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_mgxj_ge(std::string &key, std::string &value, std::set<std::string> & search);

};


#endif

