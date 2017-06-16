#ifndef __QFWS_RES_DATA_PROCESS_H__
#define __QFWS_RES_DATA_PROCESS_H__

#include "log_helper.h"
#include "ws_res_data_process.h"
#include "base_net_thread.h"
#include "common_def.h"

class tcp_connect;
class qfws_res_data_process:public ws_res_data_process
{
    public:

        ws_ser_connect(tcp_connect * t_cn);

        virtual ~ws_ser_connect();

        static ws_ser_connect * gen_connect(int fd, base_net_thread * thread);

        virtual void recv_passing_msg(base_passing_msg * p_msg);

        virtual void msg_recv_finish();
};


#endif
