#ifndef _PASSING_MSG_THRAD_H__
#define _PASSING_MSG_THRAD_H__

#include "base_def.h"
#include "base_thread.h"
#include "common_def.h"
#include "net_obj.h"

namespace MZFRAME {
	
class base_net_container;
class common_thread;
	class passing_msg_thread : public base_thread
		{
			public:
				passing_msg_thread();
				virtual ~passing_msg_thread();
				
				static int register_thread(common_thread *thread);
				
				virtual void* run();

                base_net_container * get_net_container();

                const base_net_obj * get_dest_obj(pthread_t tid);

                void set_dest_obj(pthread_t tid, base_net_obj * p_obj);
            
                NET_OBJ * gen_connect(const int fd, EPOLL_TYPE epoll_type);
                const obj_id_str & gen_id_str();
				
			protected:
                base_net_container * _net_container;
                obj_id_str _id_str;
                map<pthread_t, base_net_obj*> _thread_obj_map;
		};
}




#endif
