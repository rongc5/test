#ifndef __low_hqlow_search_index_H__
#define __low_hqlow_search_index_H__

#include "sk_def.h"
#include "common_util.h"

class low_hqlow_search_index:public base_search_index, public ToBufferMgr<std::map<std::string, std::multimap<float, std::string> > >
{
    public:
        virtual ~low_hqlow_search_index(){}

        virtual bool search(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_low_hqlow_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_low_hqlow_ge(std::string &key, std::string &value, std::set<std::string> & search);
};


#endif

