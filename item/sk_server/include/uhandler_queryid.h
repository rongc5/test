#ifndef __UHANDLER_QUERYID_H__
#define __UHANDLER_QUERYID_H__

#include "sk_def.h"
#include "stringbuffer.h"
#include "writer.h"
#include "document.h"
#include "memorystream.h"
#include "prettywriter.h"
#include "rapidjson.h"
#include <algorithm>

using namespace rapidjson;



class uhandler_queryid:public url_handler
{
    public:
        virtual void perform(http_req_head_para * req_head, std::string * recv_body, http_res_head_para * res_head, std::string * send_body);
        
        static int do_check_queryid(std::map<std::string, std::string> & url_para_map, Value & data_array, Document::AllocatorType & allocator);

        static void query_finance(std::string &id, Value & root, Document::AllocatorType & allocator);

        static void query_quotation(std::string &id, Value & root, Document::AllocatorType & allocator);

        static void query_plate(std::string &id, Value & root, Document::AllocatorType & allocator);

        static void query_blocked(std::string &id, Value & root, Document::AllocatorType & allocator);

        static void query_single(std::string &id, Value & root, Document::AllocatorType & allocator);

        static void query_single_in(std::string &id, Value & root, Document::AllocatorType & allocator);

        static void query_single_out(std::string &id, Value & root, Document::AllocatorType & allocator);

        static void query_addr(std::string &id, Value & root, Document::AllocatorType & allocator);

        static void query_technical(std::string &id, Value & root, Document::AllocatorType & allocator);

        static void query_history_single(uint32_t last_day_num, std::string &id, Value & root, Document::AllocatorType & allocator);

        static void query_sum_single(uint32_t last_day_num, std::string &id, Value & root, Document::AllocatorType & allocator);

        static void query_history_single_in(uint32_t last_day_num, std::string &id, Value & root, Document::AllocatorType & allocator);

        static void query_history_single_out(uint32_t last_day_num, std::string &id, Value & root, Document::AllocatorType & allocator);

        static void query_history_quotation(uint32_t last_day_num, std::string &id, Value & root, Document::AllocatorType & allocator);

        static void query_sum_quotation(uint32_t last_day_num, std::string &id, Value & root, Document::AllocatorType & allocator);

        static void query_history_single(std::string & history_date, std::string &id, Value & root, Document::AllocatorType & allocator);

        static void query_sum_single(std::string & history_date, std::string &id, Value & root, Document::AllocatorType & allocator);

        static void query_history_single_in(std::string & history_date, std::string &id, Value & root, Document::AllocatorType & allocator);

        static void query_history_single_out(std::string & history_date, std::string &id, Value & root, Document::AllocatorType & allocator);

        static void query_history_quotation(std::string & history_date, std::string &id, Value & root, Document::AllocatorType & allocator);

        static void query_sum_quotation(std::string & history_date, std::string &id, Value & root, Document::AllocatorType & allocator);

};

#endif
