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
		
		void add_timer(uint64_t time_length, base_timer_process *process);

		void check_timer();

        bool is_empty();

	private:
		map<uint64_t, vector<base_timer_process *> > _timer_list;
};
#endif

