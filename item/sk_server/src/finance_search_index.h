#ifndef __finance_SEARCH_INDEX_H__
#define __finance_SEARCH_INDEX_H__

#include "sk_def.h"
#include "common_util.h"

class finance_search_index:public ToBufferMgr<finance_search_item> 
{
    public:
        virtual ~finance_search_index(){}

        bool do_check_pe_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_pe_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_pb_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_pb_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_value_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_value_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_cir_value_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_cir_value_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_mgxj_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_mgxj_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_mgsy_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_mgsy_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_zysrgr_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_zysrgr_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_jlrgr_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_jlrgr_ge(std::string &key, std::string &value, std::set<std::string> & search);

};


#endif

