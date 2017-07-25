#include "web_socket_data_process.h"
#include "web_socket_msg.h"
#include "web_socket_process.h"
#include "log_helper.h"



web_socket_data_process::web_socket_data_process(web_socket_process *p):base_data_process(p->get_base_connect())
{
    _process = p;
    LOG_DEBUG("%p", this);
}

web_socket_data_process::~web_socket_data_process()
{
    LOG_DEBUG("%p", this);
    for(list<ws_msg_type>::iterator itr = _send_list.begin(); itr != _send_list.end(); ++itr)
    {
        delete itr->_p_msg;
    }	
}

void web_socket_data_process::on_handshake_ok()		
{
    LOG_DEBUG("%p", this);
}

void web_socket_data_process::on_ping(const char op_code, const string &ping_data)
{			
    LOG_DEBUG("%p", this);
}

uint64_t web_socket_data_process::get_timeout_len()
{
    return WS_CONNECT_TIMEOUT;
}

string *web_socket_data_process::get_send_buf()
{	
    string *p_ret = NULL;
    ws_msg_type tmp = get_send_msg();
    p_ret = tmp._p_msg;
    return p_ret;
}

uint64_t web_socket_data_process::get_next_send_len(int8_t &content_type)
{
    uint64_t len = 0;
    if (_send_list.begin() != _send_list.end())
    {
        len = _send_list.begin()->_p_msg->length();
        content_type = _send_list.begin()->_con_type;
    }
    return len;
}

void web_socket_data_process::put_send_msg(ws_msg_type msg)
{
    if (_send_list.begin() == _send_list.end())
    {
        _p_process->get_base_connect()->notice_send();
    }
    _send_list.push_back(msg);
}

web_socket_data_process::ws_msg_type get_send_msg()
{
    ws_msg_type ret;
    if (_send_list.begin() != _send_list.end())
    {
        ret = _send_list.front();
        _send_list.pop_front();
    }
    return ret;
}



