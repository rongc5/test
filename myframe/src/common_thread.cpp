#include "common_thread.h"

namespace MZFRAME {
	
	template<class THREAD_PROCESS>
	common_thread<THREAD_PROCESS>::common_thread()
	{
		_epoll = NULL;
		_thd_fd = 0;
	}
	
	template<class THREAD_PROCESS>
	common_thread<THREAD_PROCESS>::common_thread()
	{
		ASSERT_DO(!_epoll, delete _epoll);
	}
	
	template<class THREAD_PROCESS>
	void common_thread<THREAD_PROCESS>::init()
	{
		int fd[2], ret;
	
		ret = socketpair(AF_UNIX, 0, SOCK_STREAM, fd);
		ASSERT_DO(ret != -1, LOG_WARNING("socketpair fail errstr[%s]", strerror(errno)));
		
		_thd_fd_map[get_thread_id()] = fd[0];
		_thd_fd = fd[1];
	}
	
	template<class THREAD_PROCESS>
	void common_thread<THREAD_PROCESS>::set_process(PROCESS *p)
	{
		if (_process != NULL)
			delete _process;
		_process = p;
	}
	
	
	
	template<class THREAD_PROCESS>
	void common_thread<THREAD_PROCESS>::put_msg(pthread_t tid, obj_id_str & obj_str)
		{
				map<pthread_t, int>::iterator it;
				
				LOG_DEBUG("put_msg to tid [%lu] obj_id_str[%s]", tid, obj_str.to_str(tmp, sizeof(tmp)));
				
				it = _thd_fd_map.find(tid);
				if (it == _thd_fd_map.end()){
					char tmp[SIZE_LEN_128];
					LOG_WARNING("put_msg to tid [%lu] errstr[%s]", tid, obj_str.to_str(tmp, sizeof(tmp)));
				}	
				
				write(it->second, )				
		}
	
	
	
	
	
	
	map<pthread_t, int> net_thread::_thd_fd_map;
	
}