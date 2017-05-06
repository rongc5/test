#include "job_thread.h"
#include "job_connect.h"


bool job_thread::handle_msg(base_passing_msg * msg)
{
    if (!p_msg) {
        return true;
    }

    switch (p_msg->_op)
    {
        case PASSING_FD:
        {
            REC_OBJ<base_passing_msg> rc(p_msg);
            recv_msg_fd * r_msg = dynamic_cast<recv_msg_fd *>(p_msg);
            int fd = r_msg->fd;
            //LOG_DEBUG("recv: fd [%d]", fd);

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

    return true;
}

void job_thread::add_msg(base_passing_msg * p_msg)
{
    int index = (unsigned long) p_msg % _channel_vec.size();

    event_channel_msg * msg = _channel_msg_vec[index];
    if (msg && msg->_queue)
        msg->_queue->push<event_channel_msg>(p_msg, msg);

    return 0;
}

