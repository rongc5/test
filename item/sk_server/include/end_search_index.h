#ifndef __END_SEARCH_INDEX_H__
#define __END_SEARCH_INDEX_H__

#include "sk_def.h"
#include "common_util.h"

class end_search_index:public base_search_index, public ToBufferMgr<std::multimap<float, std::string> >
{
    public:
        virtual ~end_search_index(){}

        virtual bool search(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_end_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_end_ge(std::string &key, std::string &value, std::set<std::string> & search);

};


#endif

