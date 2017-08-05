#ifndef __WEB_SOCKET_PROCESS_H_
#define __WEB_SOCKET_PROCESS_H_

#include "base_def.h"
#include "web_socket_msg.h"
#include "base_data_process.h"


class base_connect;
class web_socket_data_process;
class web_socket_process: public base_data_process
{
	public:
		web_socket_process(base_connect *p);

		virtual ~web_socket_process();
        
        void set_process(web_socket_data_process * data_process);

		//处理接收的数据
		size_t process_recv_buf(const char *buf, const size_t len);

        virtual bool process_recv_msg(ObjId & id, normal_msg * p_msg);

		virtual string* get_send_buf();

		virtual void set_para();

		virtual void peer_close();

		virtual void send_ping(const char op_code, const string &ping_data);

		/************************************************************/
		string get_web_accept_key(const string &ws_key);

		web_socket_frame_header &get_recent_recv_frame_header();

		web_socket_frame_header &get_recent_send_frame_header();

		void notice_send();

		const string &get_recv_header();

		const string &get_send_header();

		virtual void handle_timeout(const uint32_t timer_type);

	protected:
		virtual void  parse_header() = 0;        

		virtual bool check_head_finish();

		virtual string* SEND_WB_HEAD_FINISH_PROCESS() = 0;
		virtual string* SEND_WB_INIT_STAUTS_PROCESS() = 0;			

		virtual string *SEND_WB_HANDSHAKE_OK_PROCESS();

		size_t RECV_WB_HANDSHAKE_OK_PROCESS(const char *buf, const size_t len);

		virtual size_t RECV_WB_HEAD_FINISH_PROCESS(const char *buf, const size_t len) = 0;
		virtual size_t RECV_WB_INIT_STAUTS_PROCESS(const char *buf, const size_t len) = 0;

	protected:		
		web_socket_frame_header _recent_recv_web_header;
		web_socket_frame_header _recent_send_web_header;
		string _recv_header;
		string _send_header;
		WEB_SOCKET_STATUS _wb_status;
		web_socket_data_process*  _p_data_process;
		bool _if_send_mask;		

		string _ping_data;
		list<string*> _p_tmp_str;
};

#endif
