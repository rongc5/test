#include "skhttp_req_master.h"


void handle_msg(normal_msg * p_msg)
{
}



int add_worker_thread(uint32_t thread_index)
{
    _req_thread_vec.push_back(thread_index);
}
