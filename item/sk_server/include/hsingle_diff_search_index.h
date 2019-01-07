#ifndef __hsingle_diff_SEARCH_INDEX_H__
#define __hsingle_diff_SEARCH_INDEX_H__

#include "sk_def.h"
#include "common_util.h"

class hsingle_diff_search_index:public base_search_index, public ToBufferMgr<std::vector<std::map<std::string, std::multimap<int, std::string> > > >
{
    public:
        virtual ~hsingle_diff_search_index(){}

        virtual bool search(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hsingle_diff_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hsingle_diff_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hsingle_diff_ge_num_ge(std::string &key, std::string &value, std::set<std::string> & search);
};


#endif

