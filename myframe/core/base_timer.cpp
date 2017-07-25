#include "log_helper.h"
#include "base_timer_process.h"
#include "base_timer.h"
#include "common_util.h"

base_timer::base_timer():_timer_id(0)
{
}

base_timer::~base_timer()
{
}

void base_timer::add_timer(uint64_t time_length, base_timer_process *process)
{
    LOG_DEBUG("set time_length:%lu", time_length);
    uint64_t reach_time = GetMilliSecond() + time_length;

    map<uint64_t, vector<base_timer_process *> >::iterator it;
    it  = _timer_list.find(reach_time);
    if (it != _timer_list.end()) {
        it->second.push_back(process);
    }else {
        vector<base_timer_process *> timer_vec;
        timer_vec.push_back(process);
        _timer_list[reach_time] = timer_vec;
    }
}

void base_timer::check_timer()
{
    uint64_t now = 	GetMilliSecond();

    map<uint64_t, vector<base_timer_process *> >::iterator it, itup;
    itup = _timer_list.upper_bound(now);

    vector<base_timer_process *>::iterator it_vec;
    if (itup != _timer_list.end()) {
        for (it = _timer_list.begin(); it != itup; it++) {
            for (it_vec = it->second.begin(); it_vec != it->second.end(); it_vec++) {
                (*it_vec)->handle_timeout((*it_vec)->_timer_type);
                delete (*it_vec);
            }
        }

        _timer_list.erase(_timer_list.begin(), itup);
    }
}

bool base_timer::is_empty()
{
    return _timer_list.begin() == _timer_list.end();
}

base_timer_process * base_timer::find(uint32_t timer_id)
{
    map<uint32_t, base_timer_process * >::iterator it;
    it = _timer_id_process_map.find(timer_id);
    if (it != _timer_id_process_map.end()) {
        return it->second;
    }

    return NULL;
}

uint32_t base_timer::gen_timer_id()
{
   do {
        _timer_id++;
   } while (find(timer_id)); 

   return _timer_id;
}
