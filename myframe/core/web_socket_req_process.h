#ifndef __WEB_SOCKET_REQ_PROCESS_H_
#define __WEB_SOCKET_REQ_PROCESS_H_

#include "base_def.h"
#include "web_socket_process.h"
#include "web_socket_msg.h"


class base_connect;
class web_socket_req_process:public web_socket_process
{
    public:
        web_socket_req_process(base_connect *p);

        ws_req_head_para &get_req_para();

    protected:	   
        virtual void ping_process(const int8_t op_code);//客户端不用处理服务器的ping

        virtual string* SEND_WB_HEAD_FINISH_PROCESS();

        virtual string* SEND_WB_INIT_STAUTS_PROCESS();

        virtual size_t RECV_WB_HEAD_FINISH_PROCESS(const char *buf, const size_t len);

        virtual size_t RECV_WB_INIT_STAUTS_PROCESS(const char *buf, const size_t len);

        string gen_send_http_head();

        virtual void  parse_header();

        bool check_head_finish();

    protected:
        ws_req_head_para _req_para;
        string _s_accept_key;
};



#endif

