#ifndef __zysrgr_SEARCH_INDEX_H__
#define __zysrgr_SEARCH_INDEX_H__

#include "sk_def.h"
#include "common_util.h"

class zysrgr_search_index:public base_search_index, public ToBufferMgr<std::multimap<float, std::string> >
{
    public:
        virtual ~zysrgr_search_index(){}

        virtual bool search(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_zysrgr_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_zysrgr_ge(std::string &key, std::string &value, std::set<std::string> & search);
};


#endif

