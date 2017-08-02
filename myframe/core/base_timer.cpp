#include "log_helper.h"
#include "base_timer_process.h"
#include "base_timer.h"
#include "common_util.h"

base_timer::base_timer()
{
}

base_timer::~base_timer()
{
    map<uint64_t, base_timer_process *>::iterator it;
    
    for (it = _timer_list.begin(); it != _timer_list.end(); it++) {
        delete (it->second);
    }

    _timer_list.clear();
}

uint64_t base_timer::add_timer(base_timer_process *process)
{
    LOG_DEBUG("set time_length:%lu", process->get_time_length());
    uint64_t reach_time = GetMilliSecond() + process->get_time_length();

    while (find(reach_time)) {
        reach_time += TIMER_SCALE;
    }

    process->set_reach_time(reach_time);
    _timer_list[reach_time] = process;

    return reach_time;
}

void base_timer::check_timer()
{
    uint64_t now = 	GetMilliSecond();

    map<uint64_t, base_timer_process *>::iterator it, itup;
    itup = _timer_list.upper_bound(now);

    if (itup != _timer_list.end()) {
        for (it = _timer_list.begin(); it != itup; it++) {
            it->second->handle_timeout();
            delete (it->second);
        }

        _timer_list.erase(_timer_list.begin(), itup);
    }
}

bool base_timer::is_empty()
{
    return _timer_list.begin() == _timer_list.end();
}

base_timer_process * base_timer::find(uint64_t reach_time)
{
    map<uint64_t, base_timer_process * >::iterator it;
    it = _timer_list.find(reach_time);
    if (it != _timer_list.end()) {
        return it->second;
    }

    return NULL;
}


base_timer_process * base_timer::remove_timer(uint64_t reach_time)
{
    base_timer_process * p_process = NULL;
    map<uint64_t, base_timer_process * >::iterator it;
    it = _timer_list.find(reach_time);
    if (it != _timer_list.end()) {
        p_process = it->second;
        _timer_list.erase(reach_time);
    }

    return p_process;
}
