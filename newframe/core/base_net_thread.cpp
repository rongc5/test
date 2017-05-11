#include "base_net_thread.h"
#include "base_connect.h"
#include "event_channel_msg.h"

void * base_net_thread::run()
{
    init();

    event_base_dispatch(_base);

    return NULL;
}


void base_net_thread::add_msg(base_passing_msg * p_msg)
{
    int index = (unsigned long) p_msg % _channel_msg_vec.size();

    event_channel_msg * msg = _channel_msg_vec[index];
    msg->_queue.push_back(p_msg);
    write(msg->_channelid, CHANNEL_MSG_TAG, sizeof(CHANNEL_MSG_TAG));
}

void base_net_thread::init()
{
    _base_net_thread_map[get_thread_index()] = this;
    for (int i = 0; i < _channel_num; i++) {

        int fd[2];
        int ret = socketpair(AF_UNIX,SOCK_STREAM,0,fd);
        if (ret < 0) {
            return ;
        }   

        event_channel_msg * msg = new event_channel_msg();
        msg->_net_thread = this;
        _channel_msg_vec.push_back(msg);
        msg->_channelid = fd[1];
        msg->_is_lock = true;

        struct event *_event = event_new(get_event_base(), fd[0], EV_TIMEOUT | EV_READ | EV_PERSIST, on_cb, msg);

        event_add(_event, 0);
    }
}

void base_net_thread::on_cb(int fd, short ev, void *arg)
{
    char buf[sizeof(CHANNEL_MSG_TAG)];

    typedef typename deque<base_passing_msg *>::iterator QIT;

    event_channel_msg * msg = (event_channel_msg *) arg;
    if (msg) {

        if (msg->_is_lock) {
            thread_lock lock(&_mutex);

            QIT it;
            for (it = msg->_queue.begin(); it != msg->_queue.end();) {
                if (!msg->_net_thread->handle_msg(*it)) {
                    (*it)->release();
                }
                it = msg->_queue.erase(it);
                
                recv(fd, buf, sizeof(CHANNEL_MSG_TAG), MSG_DONTWAIT);
            }

        } else {

            QIT it;
            for (it = msg->_queue.begin(); it != msg->_queue.end();) {
                if (msg->_net_thread->handle_msg(*it)) {
                    (*it)->release();
                }
                it = msg->_queue.erase(it);

                recv(fd, buf, sizeof(CHANNEL_MSG_TAG), MSG_DONTWAIT);
            }
        }
    }   
}

void base_net_thread::put_msg_ObjId(base_passing_msg * p_msg)
{
    if (!p_msg) {
        return;
    }

    base_net_thread * net_thread = get_base_net_thread_obj(p_msg->_dst_id._thread_index);

    //LOG_DEBUG("_thread_index[%d]", p_msg->_dst_id._thread_index);
    if (!net_thread) {
        REC_OBJ<base_passing_msg> rec(p_msg); 
        return;
    }

    net_thread->add_msg(p_msg);
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
        //LOG_DEBUG("will delete");
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


map<uint32_t, base_net_thread *> base_net_thread::_base_net_thread_map;
