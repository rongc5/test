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
    do
    {
        _timerid++;
        if (_timerid < TIMER_ID_BEGIN)
            _timerid = TIMER_ID_BEGIN;
        
    }while (_timerid_set.count(_timerid));

    _timerid_set.insert(_timerid);

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

    LOG_DEBUG("time_length:%u reach_time:%llu timer_id:%u _timer_type:%u", t_msg->_time_length, reach_time, t_msg->_timer_id, t_msg->_timer_type);

    _timer_list[_current].insert(make_pair(reach_time, t_msg));

    return t_msg->_timer_id;
}

void base_timer::check_timer(std::vector<uint32_t> &expect_list)
{
    uint64_t now = 	GetMilliSecond();

    std::multimap<uint64_t, std::shared_ptr<timer_msg> >::iterator it;
    it = _timer_list[_current].begin();
    std::multimap<uint64_t, std::shared_ptr<timer_msg> > & timer_list = _timer_list[_current];
    std::vector<uint64_t> tmp_vec;
    std::vector<uint64_t>::iterator ii;

    _current = 1 - _current;

    for (it = timer_list.begin(); it != timer_list.end(); it++) 
    {
        try
        {
            if (it->first > now)
                break;

            LOG_DEBUG("time_length:%u timer_id:%u _timer_type:%u", it->second->_time_length, it->second->_timer_id, it->second->_timer_type);

            _timerid_set.erase(it->second->_timer_id);
            tmp_vec.push_back(it->first);

            _net_container->handle_timeout(it->second);
        }
        catch(CMyCommonException &e) 
        {
            expect_list.push_back(it->second->_obj_id);
        }
        catch(std::exception &e) 
        {   
            expect_list.push_back(it->second->_obj_id);
        } 
    }

    for (ii = tmp_vec.begin(); ii != tmp_vec.end(); ii++)
    {
        timer_list.erase(*ii);
    }

    LOG_DEBUG("_current: size:%u idle: size:%u", _timer_list[_current].size(), _timer_list[1 - _current].size());
}

bool base_timer::is_empty()
{
    return _timer_list[_current].begin() == _timer_list[_current].end();
}

