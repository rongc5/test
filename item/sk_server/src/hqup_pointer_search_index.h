#ifndef __hqup_pointer_SEARCH_INDEX_H__
#define __hqup_pointer_SEARCH_INDEX_H__

#include "sk_def.h"
#include "common_util.h"

class hqup_pointer_search_index:public base_search_index, public ToBufferMgr<std::map<std::string, std::multimap<float, std::string> > >
{
    public:
        virtual ~hqup_pointer_search_index(){}

        virtual bool search(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hqup_pointer_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hqup_pointer_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hqup_pointer_ge_num_ge(std::string &key, std::string &value, std::set<std::string> & search);

};


#endif

