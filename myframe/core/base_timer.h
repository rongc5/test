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
		
		uint32_t add_timer(base_timer_process *process);

		void check_timer();

        base_timer_process * find(uint32_t timer_id);

        bool is_empty();

    protected:
        uint32_t gen_timer_id();

	protected:
        uint32_t _timer_id;
		map<uint64_t, vector<base_timer_process *> > _timer_list;
		map<uint32_t, base_timer_process * > _timer_id_process_map;
};
#endif

