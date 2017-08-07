#include "ws_data_process.h"
#include "web_socket_data_process.h"
#include "base_def.h"
#include "log_helper.h"
#include "web_socket_process.h"


ws_data_process::ws_data_process(web_socket_process *p):web_socket_data_process(p)
{
}

void ws_data_process::msg_recv_finish()
{
    LOG_DEBUG("recv msg:%s", _recent_msg.c_str());

    string str("");
    str.swap(_recent_msg);

    ws_msg_type msg;
    msg._p_msg = new string("I have recved");
    put_send_msg(msg);
}


