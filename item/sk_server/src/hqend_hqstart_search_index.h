#ifndef __hqend_hqstart_SEARCH_INDEX_H__
#define __hqend_hqstart_SEARCH_INDEX_H__

#include "sk_def.h"
#include "common_util.h"

class hqend_hqstart_search_index:public base_search_index, public ToBufferMgr<std::map<std::string, std::multimap<float, std::string> > >
{
    public:
        virtual ~hqend_hqstart_search_index(){}

        virtual bool search(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hqend_hqstart_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hqend_hqstart_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hqend_hqstart_ge_num_ge(std::string &key, std::string &value, std::set<std::string> & search);
};


#endif

