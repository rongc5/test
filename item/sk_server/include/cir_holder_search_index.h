#ifndef __cir_holder_SEARCH_INDEX_H__
#define __cir_holder_SEARCH_INDEX_H__

#include "sk_def.h"
#include "common_util.h"

class cir_holder_search_index:public ToBufferMgr<std::unordered_multimap<std::shared_ptr<std::string>, std::string, str_hasher, str_equaler> > 
{
    public:
        virtual ~cir_holder_search_index(){}

        bool search(std::string &key, std::string &value, search_res & search);
};


#endif

