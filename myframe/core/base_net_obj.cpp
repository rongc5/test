#include "base_net_obj.h"
#include "common_epoll.h"
#include "common_obj_container.h"
#include "common_def.h"
#include "common_exception.h"
#include "log_helper.h"
#include "base_data_process.h"
#include "common_util.h"


base_net_obj::base_net_obj()
{
    _fd = 0;
    _epoll_event = EPOLLIN | EPOLLERR | EPOLLHUP;
    _p_net_container = NULL;
    _real_net = false;
}

base_net_obj::~base_net_obj()
{

    common_epoll * p_epoll = _p_net_container->get_epoll();
    if (p_epoll) {
        p_epoll->del_from_epoll(this);
    }

    _epoll_event = 0;

    if (_fd != 0) {
        ::close(_fd);
        LOG_DEBUG("close %d", _fd);
        _fd = 0;
    }
}

void base_net_obj::set_net_container(common_obj_container *p_net_container)
{
    _p_net_container = p_net_container;
    common_epoll * p_epoll = _p_net_container->get_epoll();

    std::shared_ptr<base_net_obj> p=std::dynamic_pointer_cast<base_net_obj>(shared_from_this());

    try {
        p_epoll->add_to_epoll(p.get());
        _p_net_container->insert(p);
        add_timer();
    }
    catch (std::exception &e)
    {
        LOG_WARNING("%s", e.what());
    }
    catch (...)
    {
    }
}

bool base_net_obj::get_real_net()
{
    return _real_net;
}

void base_net_obj::set_real_net(bool real_net)
{
    
    _real_net = real_net;
    if (_real_net) {
    std::shared_ptr<base_net_obj> p=std::dynamic_pointer_cast<base_net_obj>(shared_from_this());
        LOG_DEBUG("base_net_obj:%d, .use_count:%d, _id:%d, _thread_index:%d", p, p.use_count(), p->get_id()._id, p->get_id()._thread_index);
        _p_net_container->push_real_net(p);
        LOG_DEBUG("base_net_obj:%d, .use_count:%d, _id:%d, _thread_index:%d", p, p.use_count(), p->get_id()._id, p->get_id()._thread_index);
    }
}

common_obj_container * base_net_obj::get_net_container()
{
    return _p_net_container;
}

void base_net_obj::update_event(int event)
{
    if (!_p_net_container)
    {
        return;
    }

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

void base_net_obj::handle_msg(std::shared_ptr<normal_msg> & p_msg)
{
}

void base_net_obj::notice_send()
{
}

void base_net_obj::destroy()
{
}

void base_net_obj::add_timer(std::shared_ptr<timer_msg> & t_msg)
{
    if (_p_net_container)
    {
        _p_net_container->add_timer(t_msg);
        add_timer();
    }
    else
    {
        _timer_vec.push_back(t_msg);
    }
}

void base_net_obj::add_timer()
{
    std::vector<std::shared_ptr<timer_msg> >::iterator it;

    bool flag = false;
    for (it = _timer_vec.begin(); it != _timer_vec.end(); it++)
    {
        if (_p_net_container)
        {
            _p_net_container->add_timer(*it);
            flag = true;
        }
        else 
        {
            break;
        }
    }

    if (flag)
    {
        _timer_vec.clear();
    }
}

net_addr & base_net_obj::get_peer_addr()
{
    if (!_peer_net.ip.empty() && _peer_net.port)
    {
        return _peer_net;
    }

    struct sockaddr_in sa;
    socklen_t len = sizeof(struct sockaddr_in);

    if (!getpeername(_fd, (struct sockaddr *)&sa, &len))
    {
        _peer_net.ip = inet_ntoa(sa.sin_addr);
        _peer_net.port = ntohs(sa.sin_port);
    }

    return _peer_net;
}

void base_net_obj::handle_timeout(std::shared_ptr<timer_msg> & t_msg)
{
}
