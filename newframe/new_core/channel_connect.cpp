#include "channel_connect.h"



void channel_connect::call_back(int fd, short ev, void *arg)
{
    if (_thread){
        _thread->routine_msg();
    }
}

channel_connect * channel_connect::gen_connect(int fd, base_net_thread * thread)
{
    set_unblock(fd);
    channel_connect * c_conn = new channel_connect(_fd, thread);
    c_conn->init_ev(EV_TIMEOUT | EV_READ | EV_PERSIST);
    thread->add_connect_map(c_conn);
}


