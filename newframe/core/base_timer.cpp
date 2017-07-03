#include "log_helper.h"
#include "base_timer_process.h"
#include "base_timer.h"

base_timer::base_timer(struct event_base* base)
{		
    _if_start = false;
    _base = base; 
}

base_timer::~base_timer()
{
    stop();
}

void base_timer::add_timer(const uint64_t time_length, const base_timer_process *process)
{
    LOG_DEBUG("set timer type:%u  time_length:%lu", timer_type, time_length);
    uint64_t reach_time = GetMilliSecond + time_length;

    map<uint64_t, vector<base_timer_item> >::iterator it;
    it  = _timer_list.find(reach_time);
    if (it != _timer_list.end()) {
        it->second.push_back(process);
    }else {
        vector<base_timer_process *> timer_vec;
        timer_vec.push_back(process);
        _timer_list[reach_time] = timer_vec;
    }

    if (!_if_start) {
        start();
    }
}

void base_timer::time_cb(evutil_socket_t fd, short event, void *arg)
{

    base_timer * timer = (base_timer *)arg;
    if (timer) {
        timer->check_timeout();

        struct timeval tv;
        tv.tv_sec = timer->get_check_interval()/1000;
        tv.tv_usec = timer->get_check_interval() % 1000;

        if (!timer->is_empty())
            evtimer_add(&(timer->get_event()), &tv);
    }
}


void base_timer::start(uint64_t check_interval = DEFAULT_TIMER_INTERVAL)
{
    if (_if_start)
        return;

    struct timeval tv;
    tv.tv_sec = check_interval/1000;
    tv.tv_usec = check_interval % 1000;

    evtimer_set(&_event, time_cb, this);
    event_base_set(_base, &_event);
    evtimer_add(&_event, &tv); 
    _if_start = true;
    _check_interval = check_interval;
}

void base_timer::stop()
{
    _timer_list.clear();
    if (_if_start) {
        event_del(&_event);
        _if_start = false;
    }
}

uint64_t base_timer::get_check_interval()
{
    return _check_interval;
}

struct event & base_timer::get_event()
{
    return _event;
}

void base_timer::check_timeout()
{
    uint64_t now = 	GetMilliSecond();

    map<uint64_t, vector<base_timer_process *> >::iterator it, itup;
    itup = _timer_list.upper_bound(now);

    vector<base_timer_item>::it_vec;
    if (itup != _timer_list.end()) {
        for (it = _timer_list.begin(); it < itup; it++) {
            for (it_vec = it->second.begin(); it_vec != it->second.end(); it_vec++) {
                it_vec->handle_timeout(it_vec->_timer_type);
                delete it_vec;
            }
        }

        _timer_list.erase(_timer_list.begin(), itup);
    }
}

bool base_timer::is_empty()
{
    return _timer_list.begin() == _timer_list.end();
}



