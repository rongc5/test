#include "common_connect.h"

bool common_connect::update_event(short ev)
{
    if (_ev == ev) {
        return true;
    }

    //if (_ev) 
        //event_del(&_event);

    if (_ev && event_del(&_event) == -1) 
        return false;

    event_set(&_event, _fd, ev, on_cb, this);
    event_base_set(_thread->get_event_base(), &_event);
    _ev = ev;
    if (event_add(&_event, 0) == -1) 
        return false;

    return true;
}

short common_connect::get_ev_flags()
{
    return _ev;
}

void common_connect::close()
{
    if (_fd != 0)
    {
        event_del(&_event);
        ::close(_fd);
        _fd = 0;
    }
}

int common_connect::get_sock()
{
    return _fd;
}

void common_connect::get_local_addr(sockaddr_in &addr)
{
    socklen_t len = 0;
    getsockname(_fd, (sockaddr*)&addr, &len);
}


void common_connect::on_cb(int fd, short ev, void *arg)
{
    common_connect * conn = (common_connect *) arg;
    if (conn) {
        conn->call_back(fd, ev, arg);
    }
}
