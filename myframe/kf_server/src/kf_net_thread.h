#ifndef __KF_NET_THREAD_H__
#define __KF_NET_THREAD_H__

#include "base_def.h"
#include "base_net_thread.h"
#include "common_util.h"
#include "common_def.h"
#include "ws_data_process.h"
#include "base_connect.h"
#include "web_socket_process.h"
#include "web_socket_res_process.h"
#include "log_helper.h"

class kf_net_thread:public base_net_thread
{
    public:
        virtual void handle_msg(normal_msg * p_msg)
        {
            base_connect * connect = dynamic_cast<base_connect *>(p_msg);
            if (!connect) {
                REC_OBJ<normal_msg> rc(p_msg);
                return;
            }

            connect->_msg_op = MSG_CONNECT;
            web_socket_res_process * ws_process = new web_socket_res_process(connect);
            ws_data_process * data_process = new ws_data_process(ws_process);
            ws_process->set_process(data_process);
            connect->set_process(ws_process);
            connect->set_net_container(_base_container);
        }

};




#endif
