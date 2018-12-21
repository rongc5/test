#ifndef __pe_SEARCH_INDEX_H__
#define __pe_SEARCH_INDEX_H__

#include "sk_def.h"
#include "common_util.h"

class pe_search_index:public base_search_index, public ToBufferMgr<std::multimap<int, std::string> >
{
    public:
        virtual ~pe_search_index(){}

        virtual bool search(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_pe_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_pe_ge(std::string &key, std::string &value, std::set<std::string> & search);

};


#endif

