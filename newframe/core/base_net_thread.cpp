#include "base_net_thread.h"


void * base_net_thread::run()
{
    init();

    while (get_run_flag()) {
        _base_container->obj_process();
    }

    LOG_DEBUG("obj_process over!");

    return NULL;
}

void base_net_thread::init()
{
    int fd[2];
    int ret = socketpair(AF_UNIX,SOCK_STREAM,0,fd);
    if (ret < 0) {
        LOG_WARNING("socketpair fail errstr[%s]", strerror(errno));
        return ;
    }   

    _channelid = fd[0];
    set_channelid(fd[1]);
    LOG_DEBUG("fd[%d] fd[%d]\n", fd[0], fd[1]);
    _base_net_thread_map[get_thread_index()] = this;
}


void base_net_thread::put_msg(int fd)
{
    LOG_DEBUG("_channelid[%d]", _channelid);
    thread_lock lock(&_base_net_mutex);
    _queue.push_back(fd);
    write(_channelid, "c", sizeof("c"));
    LOG_DEBUG("_channelid[%d]", _channelid);
}

void base_net_thread::routine_msg()
{
    thread_lock lock(&_base_net_mutex);
    for (dItr it = _queue.begin(); it != _queue.end();){
        handle_new_fd(*it);

        it = _queue.erase(it);
    }

    _queue.clear();
}

void base_net_thread::handle_new_fd(int fd)
{
    LOG_DEBUG("recv: fd [%d]", fd);
}

void base_net_thread::channel_cb(int fd, short ev, void *arg)
{
    base_net_thread * net_thread = (base_net_thread *)arg;

    if (net_thread){
        net_thread->routine_msg();
    }
}

void base_net_thread::set_channelid(int fd)
{
    struct event * ev = event_new(_base, fd, EV_TIMEOUT | EV_READ | EV_PERSIST, channel_cb,
                   this);

    event_add(ev, NULL);
}

