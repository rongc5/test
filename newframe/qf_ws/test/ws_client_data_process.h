#ifndef __WS_CLIENT_DATA_PROCESS_H__
#define __WS_CLIENT_DATA_PROCESS_H__

#include "log_helper.h"
#include "ws_req_data_process.h"
#include "common_def.h"

class tcp_connect;
class base_net_thread;
class ws_client_data_process: public ws_req_data_process
{
    public:

        ws_client_data_process(tcp_connect * t_cn);

        virtual ~ws_client_data_process()
        {
        }

        static void gen_connect(const string &ip, unsigned short port, base_net_thread * thread);

        virtual void on_handshake_ok();

        void send_request();

        virtual void msg_recv_finish();
};


#endif