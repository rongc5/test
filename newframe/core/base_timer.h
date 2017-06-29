#ifndef __BASE_TIMER_H_
#define __BASE_TIMER_H_

#include "common_def.h"
#include "log_helper.h"

template<class PROCESS>
class base_timer
{
		struct base_timer_item
		{
			uint32_t _timer_type;
			PROCESS  _process;
			base_timer_item()
			{
				_timer_type = 0;
			}		
		};
		
	public:
		base_timer(struct event_base* base)
		{		
            _if_start = false;
            _base = base; 
		}

		~base_timer()
		{
            stop();
		}
		
		void add_timer(const uint32_t timer_type, const uint64_t time_length, const PROCESS &process)
		{
			LOG_DEBUG("set timer type:%u  time_length:%lu", timer_type, time_length);
			uint64_t reach_time = GetMilliSecond + time_length;
			base_timer_item item;
			item._timer_type = timer_type;
			item._process = process;
            
            map<uint64_t, vector<base_timer_item> >::iterator it;
            it  = _timer_list.find(reach_time);
            if (it != _timer_list.end()) {
                it->second.push_back(base_timer_item);
            }else {
                vector<base_timer_item> timer_vec;
                timer_vec.push_back(base_timer_item);
                _timer_list[reach_time] = timer_vec;
            }

            if (!_if_start) {
                start();
            }
		}
			
        static void time_cb(evutil_socket_t fd, short event, void *arg)
        {

            base_timer<PROCESS> * timer = (base_timer<PROCESS> *)arg;
            if (timer) {
                timer->check_timeout();

                struct timeval tv;
                tv.tv_sec = timer->get_check_interval()/1000;
                tv.tv_usec = timer->get_check_interval() % 1000;

                evtimer_add(&(timer->get_event()), &tv);
            }
        }


        void start(uint64_t check_interval = DEFAULT_TIMER_INTERVAL)
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

        void stop()
        {
            _timer_list.clear();
            if (_if_start) {
                event_del(&_event);
                _if_start = false;
            }
        }

        struct timeval & get_check_interval()
        {
            return _check_interval;
        }

        struct event & get_event()
        {
            return _event;
        }

		void check_timeout()
		{
			uint64_t now = 	GetMilliSecond();
            
            map<uint64_t, vector<base_timer_item> >::iterator it, itup;
            itup = _timer_list.upper_bound(now);

             vector<base_timer_item>::it_vec;
             if (itup != _timer_list.end()) {
                 for (it = _timer_list.begin(); it < itup; it++) {
                     for (it_vec = it->second.begin(); it_vec != it->second.end(); it_vec++) {
                         it_vec->_process.handle_timeout(it_vec->_timer_type);
                     }
                 }

                 _timer_list.erase(_timer_list.begin(), itup);
             }
        }


	private:
        bool _if_start;
		map<uint64_t, vector<base_timer_item> > _timer_list;
        struct timeval  _check_interval;
        event_base* _base;
        struct event _event;
};
#endif

