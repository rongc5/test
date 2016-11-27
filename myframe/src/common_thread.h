#ifndef __COMMON_THREAD_H__
#define __COMMON_THREAD_H__

#include "def.h"

using namespace std;

class epoll_helper;

class common_thread:public base_thread
{
	public:
		common_thread();
		~common_thread();
		void init();
		int put_msg(pthread_t thd, base_obj *p_obj);
		void *run();
	protected:
		epoll_helper * _epoll;
		static map<pthread_t, int> _thd_fd_map;
		int _thd_fd;
		
};


#endif
