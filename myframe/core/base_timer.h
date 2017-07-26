#ifndef __BASE_TIMER_H_
#define __BASE_TIMER_H_

#include "common_def.h"
#include "log_helper.h"
#include "base_timer_process.h"

class base_timer
{
	public:
		base_timer();

		virtual ~base_timer();
		
		uint64_t add_timer(base_timer_process *process);

        base_timer_process * remove_timer(uint64_t reach_time);

		void check_timer();

        base_timer_process * find(uint64_t reach_time);

        bool is_empty();

	protected:
		map<uint64_t, base_timer_process *> _timer_list;
};
#endif

