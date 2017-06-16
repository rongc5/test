#ifndef __WS_REQ_DATA_PROCESS_H__
#define __WS_REQ_DATA_PROCESS_H__

#include "common_def.h"
#include "log_helper.h"
#include "ws_data_process.h"
#include "event_ws_msg.h"

class tcp_connect;
class ws_req_data_process:public ws_data_process
{
	public:
		ws_req_data_process(tcp_connect * t_cn);

		ws_req_head_para &get_req_para();

        void set_req_para(ws_req_head_para & req_para);

	protected:	   
		virtual string* SEND_WB_HEAD_FINISH_PROCESS();

		virtual string* SEND_WB_INIT_STAUTS_PROCESS();

		virtual size_t RECV_WB_HEAD_FINISH_PROCESS(const char *buf, const size_t len);

		virtual size_t RECV_WB_INIT_STAUTS_PROCESS(const char *buf, const size_t len);

		/*
		   GET /chat HTTP/1.1
Host: server.example.com
Upgrade: websocket	
Connection: Upgrade
Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==
Origin: http://example.com
Sec-WebSocket-Protocol: chat, superchat
Sec-WebSocket-Version: 13
*/      
		string gen_send_http_head();

		virtual void  parse_header();

		bool check_head_finish();

	protected:
		ws_req_head_para _req_para;
		string _s_accept_key;
};


#endif
