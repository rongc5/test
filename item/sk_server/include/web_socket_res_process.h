#ifndef __WEB_SOCKET_RES_PROCESS_H_
#define __WEB_SOCKET_RES_PROCESS_H_

#include "base_def.h"
#include "web_socket_msg.h"
#include "web_socket_process.h"
#include "base_net_obj.h"

class base_net_obj;
class web_socket_res_process:public web_socket_process
{
	public:		
		web_socket_res_process(base_net_obj *p);

		virtual ~web_socket_res_process();

        virtual void on_connect_comming();

	protected:				
		virtual std::string* SEND_WB_HEAD_FINISH_PROCESS();

		virtual std::string* SEND_WB_INIT_STAUTS_PROCESS();

		virtual size_t RECV_WB_HEAD_FINISH_PROCESS(const char *buf, const size_t len);

		virtual size_t RECV_WB_INIT_STAUTS_PROCESS(const char *buf, const size_t len);

        std::string gen_send_http_head();

		void  parse_header();

		bool check_head_finish();
		
	protected:
        std::string _s_websocket_key;
        std::string _s_ws_protocol;
        std::string _s_accept_key;
		uint32_t _wb_version;
		bool _if_upgrade;
};


#endif

