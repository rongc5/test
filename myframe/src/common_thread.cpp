#include "common_thread.h"

namespace MZFRAME {

	common_thread::common_thread()
	{
		_epoll = NULL;
		_thd_fd = 0;
	}
	
	common_thread::common_thread()
	{
		ASSERT_DO(!_epoll, delete _epoll);
	}
	
	void common_thread::init()
	{
		int fd[2], ret;
	
		ret = socketpair(AF_UNIX, 0, SOCK_STREAM, fd);
		ASSERT_DO(ret != -1, LOG_WARNING("socketpair fail errstr[%s]", strerror(errno)));
		
		_thd_fd_map[get_thread_id()] = fd[0];
		_thd_fd = fd[1];
	}
	
	map<pthread_t, int> net_thread::_thd_fd_map;
	
}