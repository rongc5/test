#ifndef __BASE_TIMER_H_
#define __BASE_TIMER_H_

#include "common_def.h"

class common_obj_container;
class base_timer
{
	public:
		base_timer(common_obj_container * net_container);

		virtual ~base_timer();
		
		uint32_t add_timer(std::shared_ptr<timer_msg> & t_msg);

		void check_timer(std::vector<uint32_t> &expect_list);

        bool is_empty();

        uint32_t gen_timerid();

	protected:
        std::multimap<uint64_t, std::shared_ptr<timer_msg> > _timer_list[2];
        std::set<uint32_t> _timerid_set;

        common_obj_container * _net_container;
        uint32_t _timerid;
        uint32_t _current;
};
#endif

