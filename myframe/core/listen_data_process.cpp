#include "listen_data_process.h"

listen_data_process::listen_data_process(void *p)
{
    _p_connect = (base_net_obj*)p;
    _thread = NULL;
}


size_t listen_data_process::process(int fd)
{
    if (_thread){
        pass_msg * p_msg = new pass_msg();

        recv_msg_fd * r_fd = new recv_msg_fd();
        r_fd->fd = fd;

        p_msg->p_msg = r_fd;
        p_msg->_obj_id = _p_connect->get_id();
        p_msg->_flag = 1;
        p_msg->_p_op = PASS_NEW_FD;

        _thread->put_msg(p_msg);
    }

    return 0;
}

void listen_data_process::set_thread(base_net_thread * thread)
{
    _thread = thread;
}



