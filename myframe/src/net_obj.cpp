#include "net_obj.h"
#include "common_epoll.h"
#include "base_net_container.h"
#include "common_def.h"
#include "common_exception.h"
#include "log_helper.h"


base_net_obj::base_net_obj()
{
    _fd = 0;
    _epoll_event = 0;
    _p_net_container = NULL;
    _p_epoll = NULL;
}

base_net_obj::~base_net_obj()
{
}


void base_net_obj::set_net_container(base_net_container *p_net_container)
{
    _p_net_container = p_net_container;
    _p_epoll = p_net_container->get_epoll();

    try {
        _p_epoll->add_to_epoll(this);
        _p_net_container->push_net_obj(this);
        PDEBUG("get_sock() %d\n", get_sock());
    }
    catch (std::exception &e)
    {
        LOG_WARNING("%s", e.what());
        PDEBUG("%s\n", e.what());
        delete this;
    }
    catch (...)
    {
        delete this;
    }
}

base_net_container * base_net_obj::get_net_container()
{
    return _p_net_container;
}


int base_net_obj::get_event()
{
    return _epoll_event;
}

int base_net_obj::get_sock()
{
    return _fd;
}

int base_net_obj::set_id(const obj_id_str & id_str)
{
    _id_str = id_str;
    return 0;
}

const obj_id_str & base_net_obj::get_id()
{
    return _id_str;
}


NET_OBJ::NET_OBJ()
{
    memset(&_peer_addr, 0, sizeof(_peer_addr));
    _p_epoll = NULL;
    _epoll_event = 0;
}
NET_OBJ::~NET_OBJ()
{		
}


void NET_OBJ::get_peer_addr(sockaddr_in &addr)
{
    addr = _peer_addr;
}

