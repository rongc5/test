#ifndef __WS_RES_DATA_PROCESS_H__
#define __WS_RES_DATA_PROCESS_H__

#include "ws_data_process.h"
#include "event_ws_msg.h"
#include "common_def.h"
#include "log_helper.h"

class tcp_connect;
class ws_res_data_process:public ws_data_process
{
    public:		
        ws_res_data_process(tcp_connect * t_cn);

        virtual ~ws_res_data_process()
        {			
        }

        virtual void handle_timeout(const uint32_t timer_type);

    protected:				
        virtual string* SEND_WB_HEAD_FINISH_PROCESS();

        virtual string* SEND_WB_INIT_STAUTS_PROCESS();

        virtual size_t RECV_WB_HEAD_FINISH_PROCESS(const char *buf, const size_t len);

        virtual size_t RECV_WB_INIT_STAUTS_PROCESS(const char *buf, const size_t len);

        /*		
                HTTP/1.1 101 Switching Protocols
Upgrade: websocket
Connection: Upgrade
Sec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=
Sec-WebSocket-Protocol: chat
*/
        string gen_send_http_head();

        void  parse_header();

        bool check_head_finish();

        virtual void on_handshake_ok();

        virtual void on_ping(const char op_code,const string &ping_data);

    protected:
        string _s_websocket_key;
        string _s_ws_protocol;
        string _s_accept_key;
        uint32_t _wb_version;
        bool _if_upgrade;
};


#endif
