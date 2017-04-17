#include "channel_connect.h"



size_t channel_connect::process_recv_buf(char *buf, size_t len)
{
    if (_thread){
        _thread->routine_msg();
    }
}

channel_connect * channel_connect::gen_connect(int fd, base_net_thread * thread)
{
    set_unblock(fd);
    channel_connect * c_conn = new channel_connect(fd, thread);
    c_conn->update_event(EV_TIMEOUT | EV_READ | EV_PERSIST);
    thread->add_connect_map(c_conn);

    return c_conn;
}


