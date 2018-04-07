#ifndef __BASE_TIMER_H_
#define __BASE_TIMER_H_

#include "common_def.h"

class base_net_container;
class base_timer
{
	public:
		base_timer(base_net_container * net_container);

		virtual ~base_timer();
		
		uint64_t add_timer(timer_msg & t_msg);

		void check_timer();

        bool is_empty();

	protected:
        std::map<uint64_t, std::vector<timer_msg> > _timer_list;
        base_net_container * _net_container;
};
#endif

