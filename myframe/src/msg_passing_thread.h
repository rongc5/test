#ifndef _MSG_PASSING_THRAD_H__
#define _MSG_PASSING_THRAD_H__

#include "base_def.h"

namespace MZFRAME {
	
	
    
	class msg_passing_thread : public base_thread
		{
			public:
				msg_passing_thread();
				virtual ~msg_passing_thread();
				
				int register_thread(common_thread *thread);
				
				virtual void* run();
				
				NET_OBJ * gen_connect(const int fd, const sockaddr_in &addr);
				
			protected:
				map<pthread_t, int> _thd_channel_map;
                base_net_container * _net_container;	
		};
}




#endif
