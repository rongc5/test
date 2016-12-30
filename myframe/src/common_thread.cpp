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
	
	
	void common_thread::set_process(PROCESS *p)
	{
		if (_process != NULL)
			delete _process;
		_process = p;
	}


	void common_thread::put_msg(pthread_t tid, obj_id_str & obj_str)
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
	
	
	 void set_thd_fd(int fd)
	 {
			_thd_fd = fd;
			struct epoll_event fds;
			
			fds.data.fd = fd;
			fds.events = EPOLLIN | EPOLLET;
			_net_container->add_event(fds);
	 }
	 
	 void common_thread::*run()
	 	{
	 		while(get_run_flag())
			{
				obj_process();
			}
	 	}
	
	
	
	
	
	
	
	
	
}