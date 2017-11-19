#ifndef __BASE_TIMER_H_
#define __BASE_TIMER_H_

#include "common_def.h"

class base_timer
{
	public:
		base_timer();

		virtual ~base_timer();
		
		uint64_t add_timer(timer_msg * t_msg);

		void check_timer();

        bool is_empty();

	protected:
		map<uint64_t, vector<timer_msg *> > _timer_list;
};
#endif

