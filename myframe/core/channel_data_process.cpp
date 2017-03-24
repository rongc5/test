#include "channel_data_process.h"

size_t channel_data_process::process_recv_buf(char *buf, size_t len)
{
    LOG_DEBUG("recv [%d]\n", len);
    size_t left_len = len;
    if (left_len > 0 && _thread)
    {
        _thread->deal_msg();
    }

    return len;
}	


void channel_data_process::set_base_net_thread(base_net_thread *thread)
{
    _thread = thread;
}


