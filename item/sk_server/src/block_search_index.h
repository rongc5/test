#ifndef __block_SEARCH_INDEX_H__
#define __block_SEARCH_INDEX_H__

#include "sk_def.h"
#include "common_util.h"

class block_search_index:public ToBufferMgr<std::unordered_set<std::string, str_hasher> >
{
    public:
        virtual ~block_search_index(){}

        virtual bool search(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_block_search(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_block(const std::string &key);

};


#endif

