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

        int do_query_id(std::map<std::string, std::string> & url_para_map, Value & root, Document::AllocatorType & allocator);

        int do_select(std::map<std::string, std::string> & url_para_map, Value & root, Document::AllocatorType & allocator);

        int do_parse_request(std::map<std::string, std::string> & url_para_map);

        void query_finance(std::string &id, Value & root, Document::AllocatorType & allocator);

        void query_quotation(std::string &id, Value & root, Document::AllocatorType & allocator);

        void query_plate(std::string &id, Value & root, Document::AllocatorType & allocator);

        void query_blocked(std::string &id, Value & root, Document::AllocatorType & allocator);

        void query_single(std::string &id, Value & root, Document::AllocatorType & allocator);

        int get_single_index(std::string &id, uint32_t num);

        void query_addr(std::string &id, Value & root, Document::AllocatorType & allocator);

        void query_history_single(uint32_t last_day_num, std::string &id, Value & root, Document::AllocatorType & allocator);

    protected:
        std::string _body;
        std::string _recv_buf;
};



#endif
