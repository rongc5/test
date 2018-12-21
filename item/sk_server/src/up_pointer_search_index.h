#ifndef __UP_POINTER_SEARCH_INDEX_H__
#define __UP_POINTER_SEARCH_INDEX_H__

#include "sk_def.h"
#include "common_util.h"

class up_pointer_search_index:public base_search_index, public ToBufferMgr<std::multimap<float, std::string> >
{
    public:
        virtual ~up_pointer_search_index(){}

        virtual bool search(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_up_pointer_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_up_pointer_ge(std::string &key, std::string &value, std::set<std::string> & search);
};


#endif

