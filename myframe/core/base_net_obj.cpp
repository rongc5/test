#include "base_net_obj.h"
#include "common_epoll.h"
#include "base_net_container.h"
#include "common_def.h"
#include "common_exception.h"
#include "log_helper.h"
#include "base_data_process.h"
#include "common_util.h"


base_net_obj::base_net_obj(const int32_t sock)
{
    _fd = sock;
    _epoll_event = EPOLLIN | EPOLLERR | EPOLLHUP;
    _p_net_container = NULL;
    _msg_op = MSG_CONNECT;
}

base_net_obj::~base_net_obj()
{
    if (_fd != 0) {
        ::close(_fd);
        _fd = 0;
    }

    common_epoll * p_epoll = _p_net_container->get_epoll();
    if (p_epoll) {
        p_epoll->del_from_epoll(this);
    }

    _epoll_event = 0;
}

void base_net_obj::set_net_container(base_net_container *p_net_container)
{
    _p_net_container = p_net_container;
    common_epoll * p_epoll = _p_net_container->get_epoll();

    try {
        p_epoll->add_to_epoll(this);
        _p_net_container->push_net_obj(this);
    }
    catch (std::exception &e)
    {
        LOG_WARNING("%s", e.what());
        delete this;
    }
    catch (...)
    {
        delete this;
    }
}

void base_net_obj::remove_net_container()
{
    common_epoll * p_epoll = _p_net_container->get_epoll();
    p_epoll->del_from_epoll(this);
    _p_net_container->remove_net_obj(this);
}

base_net_container * base_net_obj::get_net_container()
{
    return _p_net_container;
}

void base_net_obj::update_event(int event)
{
    common_epoll * p_epoll = _p_net_container->get_epoll();
    if (_epoll_event != event && p_epoll) {
        _epoll_event = event;
        p_epoll->mod_from_epoll(this);
    }
}

int base_net_obj::get_event()
{
    return _epoll_event;
}

int base_net_obj::get_sfd()
{
    return _fd;
}

void base_net_obj::set_id(const ObjId & id_str)
{
    _id_str = id_str;
}

const ObjId & base_net_obj::get_id()
{
    return _id_str;
}

bool base_net_obj::process_recv_msg(ObjId & id, normal_msg * p_msg)
{
    REC_OBJ<normal_msg> rc(p_msg);

    return true;
}

void  base_net_obj::handle_timeout(const uint32_t timer_type)
{
    
}

int base_net_obj::destroy()
{
    delete this;
    return 0;
}

