#include "skhttp_master_thread.h"


void skhttp_master_thread::handle_msg(normal_msg * p_msg)
{
    
}

void skhttp_master_thread::run_process()
{
    
}


int skhttp_master_thread::add_worker_thread(uint32_t thread_index)
{
    _req_thread_vec.push_back(thread_index);
}
