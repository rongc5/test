#include "common_connect.h"

void common_connect::init_ev(short ev)
{
    event_set(&_event, _fd, ev, on_cb, this);
    event_base_set(_thread->get_event_base(), &_event);

    event_add(&_event, 0);
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
