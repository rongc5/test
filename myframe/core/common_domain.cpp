#include "common_domain.h"
#include "common_obj_container.h"

common_domain::common_domain(common_obj_container * net_container):_net_container(net_container)
{
}

std::vector<std::string> * common_domain::get_domain(std::string & domain)
{
    auto it = _domain_cache.find(domain);
    if (it != _domain_cache.end())
    {
        return &it->second;
    }

    return NULL;
}

void common_domain::add_domain(std::string & domain, std::vector<std::string> & vip, uint64_t time_out)
{
    _domain_cache[domain] = vip;
    
    auto it = _domain_timerid_map.find(domain);
    if (it != _domain_timerid_map.end())
    {
        _timerid_domain_map.erase(it->second);
        _domain_timerid_map.erase(it);
    }

    std::shared_ptr<timer_msg> t_msg(new timer_msg);
    t_msg->_obj_id = OBJ_ID_DOMAIN;
    t_msg->_timer_type = DOMAIN_CACHE_TIMER_TYPE;
    t_msg->_time_length = time_out;

    if (_net_container)
    {
        _net_container->add_timer(t_msg);
    }

    if (t_msg->_timer_id)
    {
        _timerid_domain_map[t_msg->_timer_id] = domain;
        _domain_timerid_map[domain] = t_msg->_timer_id;
    }
}

void common_domain::delete_domain(std::string & domain)
{
    _domain_cache.erase(domain);

    auto it = _domain_timerid_map.find(domain);
    if (it != _domain_timerid_map.end())
    {
        _timerid_domain_map.erase(it->second);
        _domain_timerid_map.erase(it);
    }
}

void common_domain::handle_timeout(std::shared_ptr<timer_msg> & t_msg)
{
    if (t_msg->_timer_type == DOMAIN_CACHE_TIMER_TYPE)
    {
        auto it = _timerid_domain_map.find(t_msg->_timer_id);
        if (it != _timerid_domain_map.end())
        {
            delete_domain(it->second);
        }
    }
}

