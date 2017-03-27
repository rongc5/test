#include "test_net_thread.h"

void test_net_thread::handle_new_msg(pass_msg * p_msg)
{
    REC_OBJ<pass_msg> rec(p_msg);

    recv_msg * r_msg = dynamic_cast<recv_msg *> (p_msg->p_msg);

    //char buf[r_msg->_len + 1];
    LOG_DEBUG("recv thread: %d %s", p_msg->_src_id._thread_index, r_msg->_ptr);

    pass_msg * msg = new pass_msg();

    msg->_src_id._thread_index =  get_thread_index();
    msg->_dest_id = p_msg->_src_id;
    msg->_p_op = PASS_NEW_MSG;

    r_msg = new recv_msg();
    char * ptr = new char[SIZE_LEN_128];
    msg->p_msg = r_msg;
    r_msg->_ptr = ptr;

    snprintf(ptr, SIZE_LEN_128, "test_net_thread reply: %d", p_msg->_src_id._thread_index);

    base_net_thread::passing_msg(msg);

}



