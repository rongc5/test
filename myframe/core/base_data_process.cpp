#include "base_data_process.h"
#include "base_net_thread.h"

size_t base_data_process::process_recv_buf(char *buf, size_t len)
{
    LOG_DEBUG("%s\n", "recv msg");
    size_t left_len = len;
    while(left_len > 0)
    {
        RECV_MSG_STATUS status = RECV_MSG_HEAD;
        size_t msg_body_len = 0;
        size_t _head_len = sizeof(int); 
        if (status == RECV_MSG_HEAD)
        {
            if (left_len > _head_len)
            {
                int *p_len = (int *)buf;
                msg_body_len = *p_len;
                LOG_DEBUG("msg_body_len[%d]", msg_body_len);

                status = RECV_MSG_BODY;
            }
            else
            {
                break;
            }
        }

        if (status == RECV_MSG_BODY)
        {
            if (left_len >= _head_len + msg_body_len) {

                pass_msg * p_msg = new pass_msg();
                p_msg->_src_id = _p_connect->get_id();
                p_msg->_dest_id = _p_connect->get_id();
                p_msg->_p_op = PASS_NEW_MSG;
                p_msg->_flag = 1;
                recv_msg * r_msg = new recv_msg();
                r_msg->_flag = 0;
                p_msg->p_msg = r_msg;
                r_msg->_len = _head_len + msg_body_len;
                r_msg->_ptr = buf;

                process_recv(p_msg);

                left_len -= (_head_len + msg_body_len);
                buf = buf + _head_len + msg_body_len;
            } else {
                break;
            } 
        }				
    }

    return len - left_len;
}	

size_t base_data_process::process_recv(pass_msg * p_msg)
{
    REC_OBJ<pass_msg> rec(p_msg);

    LOG_DEBUG("%s\n", "hello world");
    return 0;
}

void base_data_process::process_send(string *p_msg)
{
    bool if_add = false;
    if (_send_list.begin() == _send_list.end())
        if_add = true;

    _send_list.push_back(p_msg);
    if (if_add)
        _p_connect->add_event(EPOLLOUT);
}


