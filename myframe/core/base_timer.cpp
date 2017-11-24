#include "log_helper.h"
#include "base_timer.h"
#include "common_util.h"
#include "base_net_thread.h"

base_timer::base_timer()
{
}

base_timer::~base_timer()
{
    map<uint64_t, vector<timer_msg *> >::iterator it;
    
    for (it = _timer_list.begin(); it != _timer_list.end(); it++) {
            vector<timer_msg *>::iterator itvec;
            for (itvec = it->second.begin(); itvec != it->second.end(); itvec++) {
                delete (*itvec);
            }
    }

    _timer_list.clear();
}

uint64_t base_timer::add_timer(timer_msg * t_msg)
{
    if (!t_msg->_reach_time) {
        t_msg->_reach_time = GetMilliSecond() + t_msg->_time_length;
    }
    LOG_DEBUG("set time_length:%lu reach_time:%lu", t_msg->_time_length, t_msg->_reach_time);

    map<uint64_t, vector<timer_msg *> >::iterator it;
    it = _timer_list.find(t_msg->_reach_time);
    if (it != _timer_list.end()) {
        it->second.push_back(t_msg);
    }else {
        vector<timer_msg *> t_vec;
        t_vec.push_back(t_msg);
        _timer_list.insert(make_pair(t_msg->_reach_time, t_vec));
    }

    return t_msg->_reach_time;
}

void base_timer::check_timer()
{
    uint64_t now = 	GetMilliSecond();

    map<uint64_t, vector<timer_msg *> >::iterator it, itup;
    itup = _timer_list.upper_bound(now);

    if (itup != _timer_list.end()) {
        for (it = _timer_list.begin(); it != itup; it++) {
            vector<timer_msg *>::iterator itvec;
            for (itvec = it->second.begin(); itvec != it->second.end(); itvec++) {
                base_net_thread::put_obj_msg((*itvec)->_id, (*itvec));
            }
        }

        _timer_list.erase(_timer_list.begin(), itup);
    }
}

bool base_timer::is_empty()
{
    return _timer_list.begin() == _timer_list.end();
}

