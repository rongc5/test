#ifndef __rsingle_SEARCH_INDEX_H__
#define __rsingle_SEARCH_INDEX_H__

#include "sk_def.h"
#include "common_util.h"

class rsingle_search_index:public ToBufferMgr<rsingle_search_item>
{
    public:
        virtual ~rsingle_search_index(){}

        bool do_check_rsingle_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_rsingle_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_rsingle_diff2_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_rsingle_diff2_ge(std::string &key, std::string &value, std::set<std::string> & search);
};


#endif

