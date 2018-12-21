#ifndef __cir_value_SEARCH_INDEX_H__
#define __cir_value_SEARCH_INDEX_H__

#include "sk_def.h"
#include "common_util.h"

class cir_value_search_index:public base_search_index, public ToBufferMgr<std::multimap<int, std::string> >
{
    public:
        virtual ~cir_value_search_index(){}

        virtual bool search(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_cir_value_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_cir_value_ge(std::string &key, std::string &value, std::set<std::string> & search);

};


#endif

