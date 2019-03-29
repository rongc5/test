#ifndef __pb_SEARCH_INDEX_H__
#define __pb_SEARCH_INDEX_H__

#include "sk_def.h"
#include "common_util.h"

class pb_search_index:public base_search_index, public ToBufferMgr<std::multimap<int, std::string> >
{
    public:
        virtual ~pb_search_index(){}

        virtual bool search(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_pb_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_pb_ge(std::string &key, std::string &value, std::set<std::string> & search);
};


#endif

