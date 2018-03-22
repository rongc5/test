#include "skhttp_master_thread.h"


skhttp_master_thread::skhttp_master_thread()
{
    _is_first = false;
}

void skhttp_master_thread::handle_msg(normal_msg * p_msg)
{
    
}

void skhttp_master_thread::run_process()
{
   if (!_is_first)
   {
       _is_first = true;
   }
}


int skhttp_master_thread::add_worker_thread(uint32_t thread_index)
{
    _req_thread_vec.push_back(thread_index);
}
