#ifndef __hquotation_SEARCH_INDEX_H__
#define __hquotation_SEARCH_INDEX_H__

#include "sk_def.h"
#include "common_util.h"

class hquotation_search_index:public ToBufferMgr<hquotation_search_item> 
{
    public:
        virtual ~hquotation_search_index(){}

        bool do_hqchange_rate_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_hqchange_rate_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_hqrange_percent_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_hqrange_percent_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_hqrange_percent_ge_num_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_hqrange_percent_le_num_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_hq_sum_change_rate_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_hq_sum_change_rate_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_hq_sum_range_percent_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_hq_sum_range_percent_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hqdown_pointer_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hqdown_pointer_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hqdown_pointer_ge_num_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hqend_start_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hqend_start_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hqend_start_ge_num_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hqup_pointer_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hqup_pointer_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hqup_pointer_ge_num_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hqend_avg_end_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hqend_avg_end_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hqend_avg_end_ge_num_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_rlow_hlow_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_rlow_hlow_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_rlow_hlow_ge_num_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_id_substr(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_rhigh_hhigh_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_rhigh_hhigh_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_rhigh_hhigh_ge_num_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hqend_end_5_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hqend_end_5_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hqend_end_5_ge_num_ge(std::string &key, std::string &value, std::set<std::string> & search);
        bool do_check_hqend_end_10_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hqend_end_10_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hqend_end_10_ge_num_ge(std::string &key, std::string &value, std::set<std::string> & search);
        bool do_check_hqend_end_20_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hqend_end_20_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hqend_end_20_ge_num_ge(std::string &key, std::string &value, std::set<std::string> & search);
        bool do_check_hqend_end_30_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hqend_end_30_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hqend_end_30_ge_num_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hqredvol_greenvol_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_check_hqredvol_greenvol_ge(std::string &key, std::string &value, std::set<std::string> & search);

        struct hidex_item
        {
            int date_index;
            int date_index_end;
            float fpoint;
            int ipoint;
            int sum;
            std::string sstr;
            hidex_item()
            {
                date_index = 0;
                date_index_end = 0;
                fpoint = 0;
                ipoint = 0;
                sum = 0;
            }
        };
};


#endif

