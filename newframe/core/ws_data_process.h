#ifndef __WS_DATA_PROCESS_H__
#define __WS_DATA_PROCESS_H__

#include "tcp_connect.h"
#include "common_def.h"
#include "event_ws_msg.h"
#include "base_data_process.h"

class ws_data_process:public base_data_process
{
    public:
        ws_data_process(tcp_connect * t_cn);

        virtual ~ws_data_process();

    public:

        virtual size_t process_recv_buf(char *buf, size_t len);

        virtual string* get_send_buf();

        virtual void reset();

        virtual void on_connect_comming();

        void send_ping(const char op_code, const string &ping_data);

        /************************************************************/
        string get_web_accept_key(const string &ws_key);

        web_socket_frame_header &get_recent_recv_frame_header();

        web_socket_frame_header &get_recent_send_frame_header();

        const string &get_recv_header();

        const string &get_send_header();

        virtual void handle_timeout(const uint32_t timer_type);

        virtual void notice_send();

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
        bool _if_send_mask;		

        string _ping_data;
        list<string*> _p_tmp_str;

        tcp_connect * _cnn;

    protected:
        virtual void on_ping(const char op_code, const string &ping_data);        

        virtual void on_handshake_ok();

        virtual void msg_recv_finish() = 0;

        virtual size_t process_recv_body(const char *buf, const size_t len);

        void put_send_msg(ws_msg_type msg);

        virtual uint64_t get_next_send_len(int8_t &content_type);

        ws_msg_type get_send_msg();

        string * get_send_data();

    protected:
        ws_msg_type _p_current_send;
        list<ws_msg_type> _send_list;
        string _recent_msg;

};


#endif
