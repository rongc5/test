#include "listen_thread.h"
#include "listen_connect.h"


void listen_thread::init(const string &ip, unsigned short port)
{
    _ip = ip;
    _port = port;

    listen_connect::gen_connect(ip, _port, this);
}

int listen_thread::add_worker_thread(base_net_thread * thread)
{
    _worker_thrds.push_back(thread);

    return 0;
}


void listen_thread::handle_new_msg(base_passing_msg * p_msg)
{

    if (!_worker_thrds.size()) {
        job_thread::handle_new_msg(p_msg);
        //LOG_DEBUG("recv_fd: %d\n", );
        return ;
    }


    {
        uint32_t index = _current_indx;
        _current_indx++;

        if (_current_indx >= _worker_thrds.size()){
            _current_indx = 0;
        }

        _worker_thrds[index]->put_msg(p_msg);
    }

}

