#ifndef __WS_CONNECT_H__
#define __WS_CONNECT_H__

#include "log_helper.h"
#include "tcp_connect.h"
#include "base_net_thread.h"
#include "common_def.h"
//#include "ws_data_process.h"
#include "event_ws_msg.h"

class base_net_thread;
class ws_connect:public tcp_connect
{
    public:

        ws_connect(int32_t sock, base_net_thread * thread):tcp_connect(sock, thread)
    {

    }

        virtual ~ws_connect();

        static ws_connect * gen_connect(int fd, base_net_thread * thread);

        virtual void recv_passing_msg(base_passing_msg * p_msg);

        size_t process_recv_buf(char *buf, size_t len);

        size_t process_s(char *buf, size_t len);

        virtual void on_connect_comming();

        void send_ping(const char op_code, const string &ping_data);

        string get_web_accept_key(const string &ws_key);

        web_socket_frame_header &get_recent_recv_frame_header();

        web_socket_frame_header &get_recent_send_frame_header();

        void notice_send();

        const string &get_recv_header();

        const string &get_send_header();

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

        //ws_data_process*  _p_data_process;

        bool _if_send_mask;		

        string _ping_data;
        list<string*> _p_tmp_str;
};


#endif
