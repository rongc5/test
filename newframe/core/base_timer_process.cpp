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

