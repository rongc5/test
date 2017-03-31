#include "base_net_thread.h"
#include "channel_connect.h"
#include "base_connect.h"
#include "test_connect.h"


void * base_net_thread::run()
{
    init();

    int i = 0;
    while (get_run_flag()) {
        event_base_dispatch(_base);
        LOG_DEBUG("event_base_dispatch %d", i);
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
    channel_connect::gen_connect(fd[1], this);

    LOG_DEBUG("fd[%d] fd[%d]\n", fd[0], fd[1]);
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
    for (deque<int>::iterator it = _queue.begin(); it != _queue.end(); ){
        handle_new_fd(*it);

        it = _queue.erase(it);
    }

    _queue.clear();
}

void base_net_thread::handle_new_fd(int fd)
{
    LOG_DEBUG("recv: fd [%d]", fd);

    set_unblock(fd);

    test_connect::gen_connect(fd, this);
}

void base_net_thread::add_connect_map(base_connect * conn)
{
    map<int, base_connect*>::iterator it;
    it = _connect_map.find(conn->get_sock());
    if (it == _connect_map.end()){
        _connect_map[conn->get_sock()] = conn;
        return;
    }

    if (it->second ==  conn) {
        return;
    }

    delete conn;
    _connect_map[conn->get_sock()] = conn;

    return;
}

void base_net_thread::destory_connect(int fd)
{
    map<int, base_connect*>::iterator it;
    it = _connect_map.find(fd);
    if (it != _connect_map.end()){
        delete it->second;
        _connect_map.erase(it);
    }
}

base_connect * base_net_thread::get_connect(int fd)
{
    map<int, base_connect*>::iterator it;
    it = _connect_map.find(fd);
    if (it != _connect_map.end()){
        return it->second;
    }

    return NULL;
}

