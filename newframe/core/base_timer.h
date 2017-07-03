#ifndef __BASE_TIMER_H_
#define __BASE_TIMER_H_

#include "common_def.h"
#include "log_helper.h"
#include "base_timer_process.h"

class base_timer
{
	public:
		base_timer(struct event_base* base);

		~base_timer();
		
		void add_timer(const uint64_t time_length, const base_timer_process *process);
			
        static void time_cb(evutil_socket_t fd, short event, void *arg);

        void start(uint64_t check_interval = DEFAULT_TIMER_INTERVAL);

        void stop();

        uint64_t  get_check_interval();

        struct event & get_event();

		void check_timeout();

        bool is_empty();

	private:
        bool _if_start;
		map<uint64_t, vector<base_timer_process *> > _timer_list;
        uint64_t  _check_interval;
        event_base* _base;
        struct event _event;
};
#endif

