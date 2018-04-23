#include "log_helper.h"
#include "base_timer.h"
#include "common_util.h"
#include "base_net_thread.h"
#include "common_obj_container.h"

base_timer::base_timer(common_obj_container * net_container)
{
    _net_container = net_container;
}

base_timer::~base_timer()
{
    _timer_list.clear();
}

uint64_t base_timer::add_timer(timer_msg & t_msg)
{
    uint64_t reach_time = GetMilliSecond() + t_msg._time_length;
    LOG_DEBUG("set time_length:%lu reach_time:%lu", t_msg._time_length, reach_time);

    std::map<uint64_t, std::vector<timer_msg> >::iterator it;
    it = _timer_list.find(reach_time);
    if (it != _timer_list.end()) {
        it->second.push_back(t_msg);
    }else {
        std::vector<timer_msg> t_vec;
        t_vec.push_back(t_msg);
        _timer_list.insert(make_pair(reach_time, t_vec));
    }

    return reach_time;
}

void base_timer::check_timer()
{
    uint64_t now = 	GetMilliSecond();

    std::map<uint64_t, std::vector<timer_msg> >::iterator it, itup;
    itup = _timer_list.upper_bound(now);

    if (itup != _timer_list.end()) {
        for (it = _timer_list.begin(); it != itup; it++) {
            std::vector<timer_msg>::iterator itvec;
            for (itvec = it->second.begin(); itvec != it->second.end(); itvec++) {
                _net_container->handle_timeout(*itvec);
            }
        }

        _timer_list.erase(_timer_list.begin(), itup);
    }
}

bool base_timer::is_empty()
{
    return _timer_list.begin() == _timer_list.end();
}

