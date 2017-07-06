#include "base_listen_process.h"

base_listen_process::listen_data_process(void *p)
{
    _p_connect = (base_net_obj*)p;
}


void base_listen_process::process(int fd)
{
    base_net_obj * net_obj = gen_net_obj(fd);

    ObjId id;
    int index = (unsigned long) net_obj % _worker_thd_vec.size();

    id._thread_index = _worker_thd_vec[index]; 
    base_net_thread::put_obj_msg(id, net_obj);

    return 0;
}

void base_listen_process::add_worker_thread(uint32_t thread_index)
{
    _worker_thd_vec.push_back(thread_index);
}

