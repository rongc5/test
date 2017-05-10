#include "client_thread.h"
#include "client_connect.h"


void client_thread::init(const string &ip, unsigned short port)
{
    _ip = ip;
    _port = port;

    client_connect::gen_connect(ip, _port, this);
}


bool client_thread::handle_msg(base_passing_msg * p_msg)
{
    if (!p_msg) {
        return true;
    }

    switch (p_msg->_op)
    {
        case PASSING_FD:
            {
                REC_OBJ<base_passing_msg> rc(p_msg);
            }
            break;
        default:
            {
                base_connect * b_con = get_connect(p_msg->_dst_id);
                if (b_con){
                    b_con->recv_passing_msg(p_msg);
                } else {
                    REC_OBJ<base_passing_msg> rc(p_msg);
                }
            }
            break;
    }

    return true;
}

