#include "qfws_res_data_process.h"
#include "log_helper.h"
#include "tcp_connect.h"
#include "ws_res_data_process.h"

qfws_res_data_process::qfws_res_data_process(tcp_connect * t_cn):ws_res_data_process(t_cn)
{
}

qfws_res_data_process::~qfws_res_data_process()
{
}

void qfws_res_data_process::msg_recv_finish()
{
   LOG_DEBUG("recv: %s", _recent_msg.c_str());

   string *tmp = new string;
   tmp->append("ok, I have recived !!!!!!!!!!!!!!");
   ws_msg_type msg;
   msg._p_msg = tmp;
   msg._con_type = 0x01;

   put_send_msg(msg);

   LOG_DEBUG("send_response");
}

