#include "test_net_thread.h"
#include "test_connect.h"


void test_net_thread::handle_new_msg(base_passing_msg * p_msg)
{
    REC_OBJ<base_passing_msg> rc(p_msg);
    recv_msg_fd * r_msg = dynamic_cast<recv_msg_fd *>(p_msg);
    int fd = r_msg->fd;
    LOG_DEBUG("recv: fd [%d]", fd);

    set_unblock(fd);

    test_connect::gen_connect(fd, this);
}

