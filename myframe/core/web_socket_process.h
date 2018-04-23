#ifndef __WEB_SOCKET_PROCESS_H_
#define __WEB_SOCKET_PROCESS_H_

#include "base_def.h"
#include "web_socket_msg.h"
#include "base_data_process.h"


class base_net_obj;
class web_socket_data_process;
class web_socket_process: public base_data_process
{
	public:
		web_socket_process(std::shared_ptr<base_net_obj> p);

		virtual ~web_socket_process();
        
        void set_process(web_socket_data_process * data_process);

		//处理接收的数据
		size_t process_recv_buf(const char *buf, const size_t len);

        virtual bool process_recv_msg(ObjId & id, std::shared_ptr<normal_msg> & p_msg);

        virtual bool handle_timeout(timer_msg & t_msg);

		virtual std::string* get_send_buf();

		virtual void peer_close();

		virtual void send_ping(const char op_code, const std::string &ping_data);

		/************************************************************/
        std::string get_web_accept_key(const std::string &ws_key);

		web_socket_frame_header &get_recent_recv_frame_header();

		web_socket_frame_header &get_recent_send_frame_header();

		void notice_send();

		const std::string &get_recv_header();

		const std::string &get_send_header();

	protected:
		virtual void  parse_header() = 0;        

		virtual bool check_head_finish();

		virtual std::string* SEND_WB_HEAD_FINISH_PROCESS() = 0;
		virtual std::string* SEND_WB_INIT_STAUTS_PROCESS() = 0;			

		virtual std::string *SEND_WB_HANDSHAKE_OK_PROCESS();

		size_t RECV_WB_HANDSHAKE_OK_PROCESS(const char *buf, const size_t len);

		virtual size_t RECV_WB_HEAD_FINISH_PROCESS(const char *buf, const size_t len) = 0;
		virtual size_t RECV_WB_INIT_STAUTS_PROCESS(const char *buf, const size_t len) = 0;

	protected:		
		web_socket_frame_header _recent_recv_web_header;
		web_socket_frame_header _recent_send_web_header;
        std::string _recv_header;
        std::string _send_header;
		WEB_SOCKET_STATUS _wb_status;
		web_socket_data_process*  _p_data_process;
		bool _if_send_mask;		

        std::string _ping_data;
        std::list<std::string*> _p_tmp_str;
};

#endif
