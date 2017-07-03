#ifndef __QFWS_RES_DATA_PROCESS_H__
#define __QFWS_RES_DATA_PROCESS_H__

#include "log_helper.h"
#include "ws_res_data_process.h"
#include "common_def.h"
#include "qfws_msg_process.h"


class tcp_connect;
class qfws_res_data_process:public ws_res_data_process
{
    public:

        qfws_res_data_process(tcp_connect * t_cn);

        virtual ~qfws_res_data_process();

        virtual void msg_recv_finish();

        void do_login(qf_req_msg & ws_msg);
        void do_online(qf_req_msg & ws_msg);

        void do_msg(qf_req_msg & ws_msg);

    protected:
         uint32_t _groupid;
         uint32_t _userid;
};


#endif
