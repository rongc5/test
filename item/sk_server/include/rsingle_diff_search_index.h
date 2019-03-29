#ifndef __rsingle_diff_SEARCH_INDEX_H__
#define __rsingle_diff_SEARCH_INDEX_H__

#include "sk_def.h"
#include "common_util.h"

class rsingle_diff_search_index:public base_search_index, public ToBufferMgr<std::vector<std::multimap<int, std::string> > >
{
    public:
        virtual ~rsingle_diff_search_index(){}

        virtual bool search(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_rsingle_diff_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_rsingle_diff_ge(std::string &key, std::string &value, std::set<std::string> & search);
};


#endif

