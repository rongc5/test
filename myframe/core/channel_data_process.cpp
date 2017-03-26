#include "channel_data_process.h"
#include "base_net_thread.h"



channel_data_process::channel_data_process(void *p):base_data_process(p)
{
}

size_t channel_data_process::process_recv_buf(char *buf, size_t len)
{
    LOG_DEBUG("recv [%d]\n", len);
    size_t left_len = len;
    if (left_len > 0)
    {


        base_net_thread * net_thread = base_net_thread::get_base_net_thread_obj(_p_connect->get_id()._thread_index);

        net_thread->routine_msg();
    }

    return len;
}


