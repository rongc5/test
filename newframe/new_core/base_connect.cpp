#include "base_connect.h"

void base_connect::init_ev(short ev)
{
    struct event * ev = event_new(_thread->get_event_base(), _fd, ev, on_cb, this); 

    event_add(ev, NULL);
}


void base_net_thread::close()
{
    if (_fd != 0)
    {
        ::close(_fd);
        _fd = 0;
        _recv_buf_len = 0;  
        event_free(_ev);
    }
}

int base_net_thread::get_sock()
{
    return _fd;
}

int base_net_thread::destroy()
{
    _thread->destroy(); 
    return 0;
}

void base_net_thread::get_local_addr(sockaddr_in &addr)
{
    socklen_t len = 0;
    getsockname(_fd, (sockaddr*)&addr, &len);
}


void base_net_thread::on_cb(int fd, short ev, void *arg)
{
    base_connect * conn = (base_connect *) arg;
    if (conn) {
        conn->call_back(fd, ev, arg);
    }
}
