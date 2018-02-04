#ifndef __SKHTTP_REQ_DATA_PROCESS_H__
#define __SKHTTP_REQ_DATA_PROCESS_H__

#include "common_def.h"
#include "http_base_process.h"
#include "http_base_data_process.h"
#include "base_net_obj.h"

class skhttp_req_data_process: public http_base_data_process
{
    public:
        skhttp_req_data_process(http_base_process * _p_process);

        virtual ~skhttp_req_data_process()
        {
            if (_req_msg)
            {
                delete _req_msg;
                _req_msg = NULL;
            }
        }

        virtual string *get_send_body(int &result);

        virtual void header_recv_finish();

        virtual void msg_recv_finish();

        virtual string * get_send_head();

        virtual size_t process_recv_body(const char *buf, size_t len, int& result);

        void set_req_msg(http_req_msg * req_msg);

        url_info & get_url_info();

        void set_url_info(url_info & info);


        static base_net_obj * gen_net_obj(http_req_msg * req_msg);

    protected:
        string _recv_buf;
        http_req_msg * _req_msg;
        url_info _url_info;

};




#endif
