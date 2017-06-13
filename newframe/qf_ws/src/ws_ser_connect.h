#ifndef __WS_SER_CONNECT_H_
#define __WS_SER_CONNECT_H_

#include "log_helper.h"
#include "ws_res_connect.h"
#include "base_net_thread.h"
#include "common_def.h"

class base_net_thread;
class ws_ser_connect:public ws_res_connect
{
    public:

        ws_ser_connect(int32_t sock, base_net_thread * thread):ws_res_connect(sock, thread)
        {

        }

        virtual ~ws_ser_connect();

        static ws_ser_connect * gen_connect(int fd, base_net_thread * thread);

        virtual void recv_passing_msg(base_passing_msg * p_msg);

        virtual void msg_recv_finish();
};


#endif
