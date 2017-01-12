#ifndef _PASSING_MSG_THRAD_H__
#define _PASSING_MSG_THRAD_H__

#include "base_def.h"
#include "base_thread.h"
#include "common_def.h"

class base_net_container;
class common_thread;
class NET_OBJ;
namespace MZFRAME {
	
	class passing_msg_thread : public base_thread
		{
			public:
				passing_msg_thread();
				virtual ~passing_msg_thread();
				
				static int register_thread(common_thread *thread);
				
				virtual void* run();

                base_net_container * get_net_container()
                {
                    return _net_container;
                }

            protected:
                NET_OBJ * gen_connect(const int fd, EPOLL_TYPE epoll_type);
				
			protected:
                base_net_container * _net_container;
		};
}




#endif
