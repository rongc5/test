#ifndef __SKHTTP_RES_DATA_PROCESS_H__
#define __SKHTTP_RES_DATA_PROCESS_H__

#include "base_def.h"
#include "http_base_process.h"
#include "http_base_data_process.h"
#include "stringbuffer.h"
#include "writer.h"
#include "document.h"
#include "memorystream.h"
#include "prettywriter.h"
#include "rapidjson.h"

using namespace rapidjson;

class skhttp_res_data_process:public http_base_data_process
{
    public:
        skhttp_res_data_process(http_base_process * _p_process);
        virtual ~skhttp_res_data_process(){}

        virtual std::string *get_send_body(int &result);

        virtual void header_recv_finish();

        virtual void msg_recv_finish();

        virtual std::string * get_send_head();

        virtual size_t process_recv_body(const char *buf, size_t len, int& result);
        
        static void gen_listen_obj(int fd, common_obj_container * net_container);

        int url_query_id(std::map<std::string, std::string> & url_para_map, Value & root, Document::AllocatorType & allocator);

        int url_select(std::map<std::string, std::string> & url_para_map, Value & root, Document::AllocatorType & allocator);

        int do_check_select(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);

        int do_parse_request(std::map<std::string, std::string> & url_para_map);

        void query_finance(std::string &id, Value & root, Document::AllocatorType & allocator);

        void query_quotation(std::string &id, Value & root, Document::AllocatorType & allocator);

        void query_plate(std::string &id, Value & root, Document::AllocatorType & allocator);

        void query_blocked(std::string &id, Value & root, Document::AllocatorType & allocator);

        void query_single(std::string &id, Value & root, Document::AllocatorType & allocator);

        void query_single_in(std::string &id, Value & root, Document::AllocatorType & allocator);

        void query_single_out(std::string &id, Value & root, Document::AllocatorType & allocator);

        void query_addr(std::string &id, Value & root, Document::AllocatorType & allocator);

        void query_history_single(uint32_t last_day_num, std::string &id, Value & root, Document::AllocatorType & allocator);

        void query_history_single_in(uint32_t last_day_num, std::string &id, Value & root, Document::AllocatorType & allocator);

        void query_history_single_out(uint32_t last_day_num, std::string &id, Value & root, Document::AllocatorType & allocator);

        void query_history_quotation(uint32_t last_day_num, std::string &id, Value & root, Document::AllocatorType & allocator);

        void query_history_single(std::string & history_date, std::string &id, Value & root, Document::AllocatorType & allocator);

        void query_history_single_in(std::string & history_date, std::string &id, Value & root, Document::AllocatorType & allocator);

        void query_history_single_out(std::string & history_date, std::string &id, Value & root, Document::AllocatorType & allocator);

        void query_history_quotation(std::string & history_date, std::string &id, Value & root, Document::AllocatorType & allocator);

    private:
        bool do_check_end_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);

        bool do_check_end_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);

        bool do_check_change_rate_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);

        bool do_check_change_rate_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);

        bool do_check_range_percent_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);

        bool do_check_range_percent_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);

        bool do_check_down_pointer_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);
        
        bool do_check_down_pointer_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);

        bool do_check_up_pointer_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);

        bool do_check_up_pointer_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);

        bool do_check_end_avg_price_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);

        bool do_check_end_avg_price_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);

        bool do_check_pe_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);

        bool do_check_pe_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);

        bool do_check_pb_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);

        bool do_check_pb_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);

        bool do_check_value_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);

        bool do_check_value_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);

        bool do_check_value_num_min(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);

        bool do_check_value_num_max(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);

        bool do_check_cir_value_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);

        bool do_check_cir_value_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);

        bool do_check_cir_value_num_min(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);

        bool do_check_cir_value_num_max(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);

        bool do_check_mgxj_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);

        bool do_check_mgxj_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);

        bool do_check_mgsy_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);

        bool do_check_mgsy_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);

        bool do_check_mgsygr_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);

        bool do_check_mgsygr_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);

        bool do_check_mgxjgr_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);

        bool do_check_mgxjgr_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);

        bool do_check_zysrgr_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);

        bool do_check_zysrgr_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);
 
        bool do_check_yylrgr_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);

        bool do_check_yylrgr_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);

        bool do_check_jlrgr_le(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);

        bool do_check_jlrgr_ge(std::map<std::string, std::string> & url_para_map, std::set<std::string> & res);

        //key like address or address_v 
        bool do_check_address(std::map<std::string, std::string> & url_para_map, const char * key, std::set<std::string> & res);
        //key like plate or plate_v
        bool do_check_plate(std::map<std::string, std::string> & url_para_map, const char * key, std::set<std::string> & res);
        //rsingle_diff0_ge, start from 0
        bool do_check_rsingle_diff2_ge(std::map<std::string, std::string> & url_para_map, uint32_t index, std::set<std::string> & res);

        bool do_check_rsingle_diff_ge(std::map<std::string, std::string> & url_para_map, uint32_t index, std::set<std::string> & res);

        void get_intersection(std::vector<std::set<std::string> > & arr, std::set<std::string> &res);

        void get_union(std::vector<std::set<std::string> > & arr, std::set<std::string> &res);

    protected:
        std::string _body;
        std::string _recv_buf;
};



#endif
