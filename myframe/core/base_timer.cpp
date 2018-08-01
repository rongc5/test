#include "log_helper.h"
#include "base_timer.h"
#include "common_util.h"
#include "base_net_thread.h"
#include "common_obj_container.h"

base_timer::base_timer(common_obj_container * net_container)
{
    _net_container = net_container;
    _timerid = TIMER_ID_BEGIN;
    _current = 0;
}

base_timer::~base_timer()
{
    _timer_list[_current].clear();
    _timer_list[1 - _current].clear();
}

uint32_t base_timer::gen_timerid()
{
    _timerid++;
    return _timerid;
}

uint32_t base_timer::add_timer(std::shared_ptr<timer_msg> & t_msg)
{
    if (t_msg->_time_length <= 0 || t_msg->_obj_id <= 0)
    {
        LOG_WARNING("add_timer failed: time_length:%u timer_id:%u _timer_type:%u", 
                t_msg->_time_length, t_msg->_timer_id, t_msg->_timer_type);
        return 0;
    }
    uint64_t reach_time = GetMilliSecond() + t_msg->_time_length;
    t_msg->_timer_id = gen_timerid();
    LOG_DEBUG("set time_length:%u reach_time:%llu timer_id:%u", t_msg->_time_length, reach_time, t_msg->_timer_id);
    //PDEBUG("set time_length:%u reach_time:%llu timer_id:%u", t_msg._time_length, reach_time, t_msg._timer_id);

    std::map<uint64_t, std::vector<std::shared_ptr<timer_msg> > >::iterator it;
    it = _timer_list[_current].find(reach_time);
    if (it != _timer_list[_current].end()) {
        it->second.push_back(t_msg);
    }else {
        std::vector<std::shared_ptr<timer_msg> > t_vec;
        t_vec.push_back(t_msg);
        _timer_list[_current].insert(make_pair(reach_time, t_vec));
    }

    return t_msg->_timer_id;
}

void base_timer::check_timer(std::vector<uint32_t> &expect_list)
{
    uint64_t now = 	GetMilliSecond();

    std::map<uint64_t, std::vector<std::shared_ptr<timer_msg> > >::iterator it, itup;
    itup = _timer_list[_current].upper_bound(now);
    it = _timer_list[_current].begin();
    std::map<uint64_t, std::vector<std::shared_ptr<timer_msg> > > & timer_list = _timer_list[_current];

    _current = 1 - _current;

    for (; it != itup && itup != timer_list.end(); it++) 
    {
        std::vector<std::shared_ptr<timer_msg> >::iterator itvec;
        for (itvec = it->second.begin(); itvec != it->second.end(); itvec++) 
        {
            try
            {
                LOG_DEBUG("set time_length:%u timer_id:%u", (*itvec)->_time_length, (*itvec)->_timer_id);
                _net_container->handle_timeout(*itvec);
            }
            catch(CMyCommonException &e) 
            {
                expect_list.push_back((*itvec)->_obj_id);
            }
            catch(std::exception &e) 
            {   
                expect_list.push_back((*itvec)->_obj_id);
            } 
        }
    }

    if (itup != timer_list.end())
    {
        timer_list.erase(timer_list.begin(), itup);
    }
}

bool base_timer::is_empty()
{
    return _timer_list[_current].begin() == _timer_list[_current].end();
}

