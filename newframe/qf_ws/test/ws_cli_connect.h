#ifndef __WS_CLI_CONNECT_H_
#define __WS_CLI_CONNECT_H_

#include "log_helper.h"
#include "ws_req_connect.h"
#include "common_def.h"

class base_net_thread;
class ws_cli_connect: public ws_req_connect
{
    public:

        ws_cli_connect(int32_t sock, base_net_thread * thread):ws_req_connect(sock, thread)
        {

        }

        virtual ~ws_cli_connect()
        {
        }

        static ws_cli_connect * gen_connect(const string &ip, unsigned short port, base_net_thread * thread);

        virtual void on_handshake_ok();

        void send_request();

        virtual void msg_recv_finish();
};


#endif
