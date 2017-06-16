#include "ws_ser_connect.h"
#include "log_helper.h"


ws_ser_connect::~ws_ser_connect()
{
}

ws_ser_connect * ws_ser_connect::gen_connect(int fd, base_net_thread * thread)
{
    //set_unblock(fd);
    ws_ser_connect * c_conn = new ws_ser_connect(fd, thread);
    c_conn->update_event(EV_TIMEOUT | EV_READ | EV_PERSIST);
    thread->add_connect_map(c_conn);

    return c_conn;
}

void ws_ser_connect::recv_passing_msg(base_passing_msg * p_msg)
{
    REC_OBJ<base_passing_msg> rc(p_msg);
    
    //process_send_buf((char *)rs_http->res_buf.c_str(), rs_http->res_buf.length());
}

void ws_ser_connect::msg_recv_finish()
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

