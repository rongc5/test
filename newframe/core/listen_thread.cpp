#include "listen_thread.h"
#include "listen_connect.h"

void listen_thread::init(const string &ip, unsigned short port)
{
    _ip = ip;
    _port = port;

    listen_connect::gen_connect(ip, _port, this);
}


bool listen_thread::handle_msg(base_passing_msg * p_msg)
{
    int index = (unsigned long)p_msg  % _listen_thread_vec.size();
    _listen_thread_vec[index]->add_msg(p_msg);

    return true;
}

void listen_thread::add_msg(base_passing_msg * p_msg)
{
    REC_OBJ<base_passing_msg> rc(p_msg);

    return;
}


int listen_thread::add_worker_thread(base_net_thread * thread)
{
    _listen_thread_vec.push_back(thread);

    return 0;
}
