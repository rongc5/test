#include "listen_thread.h"
#include "listen_connect.h"
#include "job_thread.h"

void listen_thread::init(const string &ip, unsigned short port)
{
    _ip = ip;
    _port = port;

    listen_connect::gen_connect(ip, _port, this);
}


bool listen_thread::handle_msg(base_passing_msg * p_msg)
{
    job_thread::put_msg(p_msg);

    return true;
}

void listen_thread::add_msg(base_passing_msg * p_msg)
{
    REC_OBJ<base_passing_msg> rc(p_msg);

    return;
}

