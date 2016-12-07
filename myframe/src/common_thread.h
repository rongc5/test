#ifndef __COMMON_THREAD_H__
#define __COMMON_THREAD_H__

#include "def.h"


namespace MZFRAME {

using namespace std;

class epoll_helper;






template<class THREAD_PROCESS>
class common_thread:public base_thread
{
	public:
		common_thread();
		~common_thread();
		void init();
		int put_msg(pthread_t thd, base_obj *p_obj);
		void *run();
		void set_process(PROCESS *p);
		void obj_process();
		void put_msg(pthread_t tid, obj_id_str & obj_str);
		void put_msg(obj_id_str & obj_str);
		
	protected:
		
		static map<pthread_t, int> _thd_fd_map;
		int _thd_fd;
		uint64_t _obj_id;
		
		
		THREAD_PROCESS *_process;
};

}
#endif
