#ifndef __WEB_SOCKET_DATA_PROCESS_H_
#define __WEB_SOCKET_DATA_PROCESS_H_

#include "base_def.h"
#include "base_data_process.h"
#include "web_socket_msg.h"

class web_socket_process;
class web_socket_data_process:public base_data_process
{
    public:
        web_socket_data_process(web_socket_process *p);

        virtual ~web_socket_data_process();

        virtual void on_handshake_ok();

        virtual void on_ping(const char op_code, const string &ping_data);

        virtual uint64_t get_timeout_len();

        virtual uint64_t get_next_send_len(int8_t &content_type);
        virtual string *get_send_buf();
        virtual void msg_recv_finish() = 0;

         virtual size_t process_recv_buf(const char *buf, size_t len);

    protected:
        void put_send_msg(ws_msg_type msg);

        ws_msg_type get_send_msg();

    protected:
        web_socket_process *_process;
        ws_msg_type _p_current_send;
        list<ws_msg_type> _send_list;
        string _recent_msg;
};


#endif

