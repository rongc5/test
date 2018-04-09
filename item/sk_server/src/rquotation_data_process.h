#ifndef __RQUOTATION_DATA_PROCESS_H__
#define __RQUOTATION_DATA_PROCESS_H__

#include "common_def.h"
#include "http_base_process.h"
#include "http_base_data_process.h"
#include "base_net_obj.h"
#include "sk_def.h"

class rquotation_data_process: public http_base_data_process
{
    public:
        rquotation_data_process(http_base_process * _p_process);

        virtual ~rquotation_data_process()
        {
        }

        virtual std::string *get_send_body(int &result);

        virtual void header_recv_finish();

        virtual void msg_recv_finish();

        virtual std::string * get_send_head();

        virtual size_t process_recv_body(const char *buf, size_t len, int& result);

        virtual bool process_recv_msg(ObjId & id, normal_msg * p_msg);

        void set_req_msg(http_req_msg & req_msg);

        url_info & get_url_info();

        void set_url_info(url_info & info);

        static base_net_obj * gen_net_obj(url_info & info, http_req_msg & req_msg);

        virtual bool handle_timeout(timer_msg & t_msg);

    protected:
        std::string _recv_buf;
        http_req_msg _req_msg;
        url_info _url_info;
        bool _is_ok;

};




#endif
