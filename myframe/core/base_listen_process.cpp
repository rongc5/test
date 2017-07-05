#include "base_listen_process.h"

base_listen_process::listen_data_process(void *p)
{
    _p_connect = (base_net_obj*)p;
}


size_t base_listen_process::process(int fd)
{
    pass_msg * p_msg = new pass_msg();

    recv_msg_fd * r_fd = new recv_msg_fd();
    r_fd->fd = fd;

    p_msg->p_msg = r_fd;
    p_msg->_src_id = _p_connect->get_id();
    p_msg->_dest_id = _p_connect->get_id();
    LOG_DEBUG("_p_connect->get_id thread_index[%d]", p_msg->_dest_id._thread_index);
    p_msg->_flag = 1;
    p_msg->_p_op = PASS_NEW_FD;

    base_net_thread::passing_msg(p_msg);

    return 0;
}



