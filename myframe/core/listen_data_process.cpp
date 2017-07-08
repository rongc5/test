#include "listen_data_process.h"
#include "base_listen_thread.h"

listen_data_process::listen_data_process(void *p)
{
    _p_connect = (base_net_obj*)p;
    _listen_thread = NULL;
}


void listen_data_process::process(int fd)
{
    base_net_obj * net_obj = _listen_thread->gen_listen_obj(fd);

    ObjId id;
    int index = (unsigned long) net_obj % _worker_thd_vec.size();

    id._thread_index = _worker_thd_vec[index]; 
    base_net_thread::put_obj_msg(id, net_obj);
}

void listen_data_process::add_worker_thread(uint32_t thread_index)
{
    _worker_thd_vec.push_back(thread_index);
}

void listen_data_process::set_listen_thread(base_listen_thread * thread)
{
    _listen_thread = thread;
}

base_listen_thread * listen_data_process::get_listen_thread()
{
    return _listen_thread;
}
