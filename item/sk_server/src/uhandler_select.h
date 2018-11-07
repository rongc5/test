#ifndef __UHANDLER_SELECT_H__
#define __UHANDLER_SELECT_H__

#include "sk_def.h"


enum SETS_OP_TRPE
 {
     SETS_OP_INTERSECTION,
     SETS_OP_UNION
 };

class uhandler_select:public url_handler
{
    public:
        virtual void perform(http_req_head_para * req_head, std::string * recv_body, http_res_head_para * res_head, std::string * send_body);
        
        static int do_parse_request(std::string * _recv_buf, std::map<std::string, std::string> & url_para_map);
        
        static int do_check_select(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);
        
        static bool do_check_end_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_end_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_change_rate_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_change_rate_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_range_percent_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_history_sum_range_percent_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_history_has_range_percent_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_history_range_percent_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_range_percent_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_history_sum_range_percent_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_history_range_percent_ge_num_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_history_range_percent_ge_num_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_history_has_range_percent_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE);

        static bool do_check_history_range_percent_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_down_pointer_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);
        
        static bool do_check_down_pointer_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_up_pointer_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_up_pointer_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_end_avg_price_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_end_avg_price_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_pe_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_pe_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_pb_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_pb_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_value_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_value_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_value_num_min(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_value_num_max(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_cir_value_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_cir_value_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_cir_value_num_min(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_cir_value_num_max(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_mgxj_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_mgxj_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_mgsy_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_mgsy_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_mgsygr_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_mgsygr_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_mgxjgr_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_mgxjgr_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_zysrgr_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_zysrgr_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);
 
        static bool do_check_yylrgr_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_yylrgr_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_jlrgr_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_jlrgr_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        //key like address or address_v 
        static bool do_check_address(std::map<std::string, std::string> & url_para_map, const char * key, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);
        //key like plate or plate_v
        static bool do_check_plate(std::map<std::string, std::string> & url_para_map, const char * key, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);
        //rsingle_diff0_ge, start from 0
        static bool do_check_rsingle_diff2_ge(std::map<std::string, std::string> & url_para_map, uint32_t index, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_rsingle_diff_ge(std::map<std::string, std::string> & url_para_map, uint32_t index, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_hsingle_sum_diff_ge(std::map<std::string, std::string> & url_para_map, uint32_t index, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_hsingle_diff_ge(std::map<std::string, std::string> & url_para_map, uint32_t index, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_id_substr(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);

        static bool do_check_end_hqend_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res, std::set<std::string> & search, SETS_OP_TRPE et);
        
};

#endif
