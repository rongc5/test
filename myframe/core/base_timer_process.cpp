#include "base_timer_process.h"
#include "common_def.h"
#include "log_helper.h"

base_timer_process::~base_timer_process()
{
    LOG_DEBUG("destory:%p", this);
}

void base_timer_process::handle_timeout(const uint32_t timer_type)
{
    LOG_DEBUG("handle_timeout");
}

void base_timer_process::init(uint32_t timer_type, uint64_t time_length)
{
    _timer_type = timer_type;
    _time_length = time_length;
}

void base_timer_process::set_reach_time(uint64_t reach_time)
{
    _reach_time = reach_time;
}

uint64_t base_timer_process::get_reach_time()
{
    return _reach_time;
}

uint64_t base_timer_process::get_time_length()
{
    return _time_length;
}

uint32_t base_timer_process::get_timer_type()
{
    return _timer_type;
}
