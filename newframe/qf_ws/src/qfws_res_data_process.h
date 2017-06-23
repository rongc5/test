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

        bool parse_request(qf_ws_msg & ws_msg);


};


#endif
