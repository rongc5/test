#include "job_thread.h"
#include "job_connect.h"


void job_thread::handle_new_msg(base_passing_msg * p_msg)
{
    if (!p_msg) {
        return;
    }

    switch (p_msg->_op)
    {
        case PASSING_FD:
        {
            REC_OBJ<base_passing_msg> rc(p_msg);
            recv_msg_fd * r_msg = dynamic_cast<recv_msg_fd *>(p_msg);
            int fd = r_msg->fd;
            LOG_DEBUG("recv: fd [%d]", fd);

            set_unblock(fd);

            job_connect::gen_connect(fd, this);
        }
        break;
        default :
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


}

