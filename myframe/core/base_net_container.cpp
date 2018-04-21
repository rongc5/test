#include "common_epoll.h"
#include "base_net_container.h"
#include "base_net_thread.h"
#include "base_timer.h"


base_net_container::base_net_container(base_net_thread * thread)
{
    _net_thread = thread;
    _p_epoll = NULL;
    _timer = NULL;
}

base_net_container::~base_net_container()
{
    if (_p_epoll != NULL)
        delete _p_epoll;

    if (_timer) {
        delete _timer;
    }
}


common_epoll * base_net_container::get_epoll()
{
    return _p_epoll;
}

base_timer * base_net_container::get_timer()
{
    return _timer;
}

void base_net_container::init(const uint32_t epoll_size)
{
    _p_epoll = new common_epoll();
    _p_epoll->init(epoll_size);

    _timer = new base_timer(this);

    _id_str._id = OBJ_ID_BEGIN;
}

const ObjId & base_net_container::gen_id_str()
{
    uint32_t obj_id = _id_str._id;
    uint32_t thread_index = _net_thread->get_thread_index();
    _id_str._thread_index = thread_index;
    do {
        obj_id++;
        _id_str._id = obj_id;
    } while (find(&_id_str) || obj_id <= OBJ_ID_BEGIN);

    return _id_str;
}

base_net_thread * base_net_container::get_net_thread()
{
    return _net_thread;
}

void base_net_container::add_timer(timer_msg & t_msg)
{
    if (_timer)
        _timer->add_timer(t_msg); 
}

void base_net_container::handle_timeout(timer_msg & t_msg)
{
    
    bool flag = false;
    if (t_msg._id._id <= OBJ_ID_BEGIN)
    {
        flag = _net_thread->handle_timeout(t_msg);
        return;
    }

    std::shared_ptr<base_net_obj>  net_obj = find(&t_msg._id);
    if (!net_obj)
    {
        return;
    }

    flag = net_obj->handle_timeout(t_msg);
    if (!flag)
    {
        return;
    }
}


