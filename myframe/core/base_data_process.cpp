#include "base_net_obj.h"
#include "base_data_process.h"
#include "log_helper.h"


base_data_process::base_data_process(std::shared_ptr<base_net_obj> p)
{
    _p_connect = p;
    LOG_DEBUG("%p", this);
}

base_data_process::~base_data_process()
{
    LOG_DEBUG("%p", this);
    clear_send_list();
}

void base_data_process::peer_close()
{
    LOG_DEBUG("%p", this);
}

std::string * base_data_process::get_send_buf()
{
    LOG_DEBUG("%p", this);
    if (_send_list.begin() == _send_list.end()) {
        LOG_DEBUG("_send_list is empty");
        return NULL;
    }

    std::string *p = *(_send_list.begin());
    _send_list.erase(_send_list.begin());

    return p;
}

void base_data_process::reset()
{
    clear_send_list();
    LOG_DEBUG("%p", this);
}

size_t base_data_process::process_recv_buf(const char *buf, size_t len)
{
    LOG_DEBUG("%p", this);

    return len;
}

void base_data_process::handle_msg(std::shared_ptr<normal_msg> & p_msg)
{
    LOG_DEBUG("%p", this);
}

void base_data_process::clear_send_list()
{
    for (std::list<std::string*>::iterator itr = _send_list.begin(); itr != _send_list.end(); ++itr)
    {
        delete *itr;
    }

    _send_list.clear();
}

void base_data_process::put_send_buf(std::string * str)
{
    _send_list.push_back(str);
    if (auto sp = _p_connect.lock())
    {
        sp->notice_send();  
    }
}

std::shared_ptr<base_net_obj>  base_data_process::get_base_net()
{
    return _p_connect.lock();
}

void base_data_process::destroy()
{

}

void base_data_process::add_timer(std::shared_ptr<timer_msg> & t_msg)
{
    if (auto sp = _p_connect.lock())
        sp->add_timer(t_msg);
}

void base_data_process::handle_timeout(std::shared_ptr<timer_msg> & t_msg)
{
}
