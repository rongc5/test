#include "base_connect.h"
#include "base_data_process.h"
#include "log_helper.h"


base_data_process::base_data_process(base_connect *p)
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

string * base_data_process::get_send_buf()
{
    LOG_DEBUG("%p", this);
    if (_send_list.begin() == _send_list.end()) {
        LOG_DEBUG("_send_list is empty");
        return NULL;
    }

    string *p = *(_send_list.begin());
    _send_list.erase(_send_list.begin());

    return p;
}

void base_data_process::reset()
{
    clear_send_list();
    LOG_DEBUG("%p", this);
}

void base_data_process::set_para()
{
    LOG_DEBUG("%p", this);
}

void base_data_process::on_connect_comming()
{
    LOG_DEBUG("%p", this);
}

size_t base_data_process::process_recv_buf(const char *buf, size_t len)
{
    LOG_DEBUG("%p", this);

    return len;
}

bool base_data_process::process_recv_msg(ObjId & id, normal_msg * p_msg)
{
    LOG_DEBUG("%p", this);

    return true;
}

void base_data_process::clear_send_list()
{
    for (list<string*>::iterator itr = _send_list.begin(); itr != _send_list.end(); ++itr)
    {
        delete *itr;
    }

    _send_list.clear();
}

void base_data_process::put_send_buf(string * str)
{
    _send_list.push_back(str);
    _p_connect->notice_send();  
}

base_connect * base_data_process::get_base_connect()
{
    return _p_connect;
}

