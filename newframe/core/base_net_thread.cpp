#include "base_net_thread.h"
#include "channel_connect.h"
#include "base_connect.h"


void * base_net_thread::run()
{
    init();

    event_base_dispatch(_base);

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
    _base_net_thread_map[get_thread_index()] = this;

    LOG_DEBUG("fd[%d] fd[%d]\n", fd[0], fd[1]);
}


void base_net_thread::put_msg(base_passing_msg * p_msg)
{
    if (!p_msg) {
        return;
    }

    LOG_DEBUG("_channelid[%d]", _channelid);
    thread_lock lock(&_base_net_mutex);
    _queue.push_back(p_msg);
    write(_channelid, "c", sizeof("c"));
    LOG_DEBUG("_channelid[%d]", _channelid);
}

void base_net_thread::routine_msg()
{
    thread_lock lock(&_base_net_mutex);
    for (deque<base_passing_msg *>::iterator it = _queue.begin(); it != _queue.end(); ){
        handle_new_msg(*it);

        it = _queue.erase(it);
    }

    _queue.clear();
}

void base_net_thread::add_connect_map(base_connect * conn)
{
    map<ObjId, base_connect*>::iterator it;
    it = _connect_map.find(conn->get_id());
    if (it == _connect_map.end()){
        _connect_map[conn->get_id()] = conn;
        return;
    }

    if (it->second ==  conn) {
        return;
    }

    delete conn;
    _connect_map[conn->get_id()] = conn;

    return;
}

void base_net_thread::destory_connect(ObjId id)
{
    map<ObjId, base_connect*>::iterator it;
    it = _connect_map.find(id);
    if (it != _connect_map.end()){
        LOG_DEBUG("will delete");
        _connect_map.erase(it);
    }
}

base_connect * base_net_thread::get_connect(ObjId id)
{
    map<ObjId, base_connect*>::iterator it;
    it = _connect_map.find(id);
    if (it != _connect_map.end()){
        return it->second;
    }

    return NULL;
}

const ObjId & base_net_thread::gen_id_str()
{
    _id_str._thread_index = get_thread_index();
    _id_str._id++;

    return _id_str;
}

base_net_thread * base_net_thread::get_base_net_thread_obj(uint32_t thread_index)
{
    bntMapIter it = _base_net_thread_map.find(thread_index);
    if (it != _base_net_thread_map.end()){
        return it->second;
    }

    return NULL;
}

void base_net_thread::passing_msg(base_passing_msg * p_msg)
{
    if (!p_msg) {
        return;
    }

    base_net_thread * net_thread = get_base_net_thread_obj(p_msg->_dst_id._thread_index);

    LOG_DEBUG("_thread_index[%d]", p_msg->_dst_id._thread_index);
    if (!net_thread) {
        REC_OBJ<base_passing_msg> rec(p_msg); 
        return;
    }

    net_thread->put_msg(p_msg);
}


map<uint32_t, base_net_thread *> base_net_thread::_base_net_thread_map;
