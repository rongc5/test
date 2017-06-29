#ifndef __BASE_NET_OBJ_TIMER_PROCESS_H__
#define __BASE_NET_OBJ_TIMER_PROCESS_H__

#include "common_def.h"


class base_net_thread;
class base_net_obj_timer_process
{
	public:
		base_net_obj_timer_process(ObjId  id, base_net_thread *thread);

		base_net_obj_timer_process();
		
		void handle_timeout(const uint32_t timer_type);
	private:
		base_net_thread *_thread;
		ObjId  _id;
};




#endif


