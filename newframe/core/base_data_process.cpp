#include "common_def.h"
#include "base_data_process.h"
#include "log_helper.h"

base_data_process::base_data_process()
{
    LOG_DEBUG("%p", this);
}

base_data_process::~base_data_process()
{
    LOG_DEBUG("%p", this);
}

void base_data_process::err_close()
{
    LOG_DEBUG("%p", this);
}

string *base_data_process::get_send_buf()
{
    LOG_DEBUG("%p", this);

    return NULL;
}

void base_data_process::reset()
{
    LOG_DEBUG("%p", this);
}

void base_data_process::set_para()
{ 
    LOG_DEBUG("%p", this);
}

void base_data_process::on_connect_comming()
{
    LOG_DEBUG("%p", this);
}

size_t base_data_process::process_recv_buf(char *buf, size_t len)
{
    LOG_DEBUG("%p", this);

    return 0;
}

size_t base_data_process::process_recv_msg(base_passing_msg * p_msg)
{
    REC_OBJ<base_passing_msg> rc(p_msg);

    LOG_DEBUG("%p", this);
    
    return 0;
}

void base_data_process::notice_send()
{
    LOG_DEBUG("%p", this);
}

void base_data_process::handle_timeout(const uint32_t timer_type)
{ 
    LOG_DEBUG("%p", this);
}


