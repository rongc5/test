#ifndef __SKHTTP_RES_DATA_PROCESS_H__
#define __SKHTTP_RES_DATA_PROCESS_H__

#include "base_def.h"
#include "http_base_process.h"
#include "http_base_data_process.h"


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

        void do_query_id(url_info & u_info, std::string & res);

        void do_select(url_info & u_info, std::string & res);

        void do_req_err(std::string & res);

    protected:
        std::string _body;
        std::string _recv_buf;
};



#endif
