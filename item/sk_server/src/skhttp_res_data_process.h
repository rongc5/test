#ifndef __SKHTTP_RES_DATA_PROCESS_H__
#define __SKHTTP_RES_DATA_PROCESS_H__

#include "base_def.h"
#include "http_base_process.h"
#include "http_base_data_process.h"
#include "sk_def.h"

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
        
        void reg_handler(std::string & url, std::shared_ptr<url_handler> & handler);

    private:
     
        void get_intersection(std::vector<std::set<std::string> > & arr, std::set<std::string> &res);

        void get_union(std::vector<std::set<std::string> > & arr, std::set<std::string> &res);

    protected:
        std::string _body;
        std::string _recv_buf;
        
        std::map<std::string, std::shared_ptr<url_handler> > _uhandler_map;
};



#endif
