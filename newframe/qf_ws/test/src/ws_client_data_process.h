#ifndef __WS_CLIENT_DATA_PROCESS_H__
#define __WS_CLIENT_DATA_PROCESS_H__

#include "log_helper.h"
#include "ws_req_data_process.h"
#include "common_def.h"
#include "qfws_msg_process.h"
#include "base_timer.h"
#include "base_net_obj_timer_process.h"



class tcp_connect;
class base_net_thread;
class ws_client_data_process: public ws_req_data_process
{
    public:

        ws_client_data_process(tcp_connect * t_cn);

        virtual ~ws_client_data_process()
        {
        }

        static ws_client_data_process * gen_connect(const string &ip, unsigned short port, base_net_thread * thread);

        virtual void on_handshake_ok();

        void login_request();
        void do_login_response(qf_res_msg & ws_msg);

        void online_request();
        void do_online_response(qf_res_msg & ws_msg);

        void send_request(const char * buf);

        virtual void msg_recv_finish();

        virtual void add_timer(const uint32_t timer_type, const uint64_t time_length);
};


#endif
