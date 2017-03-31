#include "base_connect.h"

void base_connect::init_ev(short ev)
{
    event_set(&_event, _fd, ev, on_cb, this);
    event_base_set(_thread->get_event_base(), &_event);

    event_add(&_event, 0);
}


void base_connect::close()
{
    if (_fd != 0)
    {
        event_del(&_event);
        ::close(_fd);
        _fd = 0;
    }
}

int base_connect::get_sock()
{
    return _fd;
}

int base_connect::destroy()
{
    _thread->destory_connect(_fd); 
    return 0;
}

void base_connect::get_local_addr(sockaddr_in &addr)
{
    socklen_t len = 0;
    getsockname(_fd, (sockaddr*)&addr, &len);
}


void base_connect::on_cb(int fd, short ev, void *arg)
{
    base_connect * conn = (base_connect *) arg;
    if (conn) {
        try {
        conn->call_back(fd, ev, arg);
        } catch (std::exception & e) {
            LOG_WARNING("caught exception info:%s", e.what());
            conn->destroy();
        }catch (...) {
            LOG_WARNING("caught unknown exception");
            conn->destroy();
        }
    }
}
