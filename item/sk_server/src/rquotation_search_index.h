#ifndef __rquotation_SEARCH_INDEX_H__
#define __rquotation_SEARCH_INDEX_H__

#include "sk_def.h"
#include "common_util.h"

class rquotation_search_index:public ToBufferMgr<rquotation_search_item> 
{
    public:
        virtual ~rquotation_search_index(){}

        bool do_check_end_le(std::string &key, std::string &value, search_res & search);

        bool do_check_end_ge(std::string &key, std::string &value, search_res & search);

        bool do_check_change_rate_le(std::string &key, std::string &value, search_res & search);

        bool do_check_change_rate_ge(std::string &key, std::string &value, search_res & search);

        bool do_check_range_percent_le(std::string &key, std::string &value, search_res & search);

        bool do_check_range_percent_ge(std::string &key, std::string &value, search_res & search);

        bool do_check_down_pointer_le(std::string &key, std::string &value, search_res & search);

        bool do_check_down_pointer_ge(std::string &key, std::string &value, search_res & search);

        bool do_check_up_pointer_le(std::string &key, std::string &value, search_res & search);

        bool do_check_up_pointer_ge(std::string &key, std::string &value, search_res & search);

        bool do_check_end_avg_end_le(std::string &key, std::string &value, search_res & search);

        bool do_check_end_avg_end_ge(std::string &key, std::string &value, search_res & search);

        bool do_check_end_end5_le(std::string &key, std::string &value, search_res & search);

        bool do_check_end_end5_ge(std::string &key, std::string &value, search_res & search);

        bool do_check_end_end10_le(std::string &key, std::string &value, search_res & search);

        bool do_check_end_end10_ge(std::string &key, std::string &value, search_res & search);

        bool do_check_end_end20_le(std::string &key, std::string &value, search_res & search);

        bool do_check_end_end20_ge(std::string &key, std::string &value, search_res & search);

        bool do_check_end_end30_le(std::string &key, std::string &value, search_res & search);

        bool do_check_end_end30_ge(std::string &key, std::string &value, search_res & search);


};


#endif

