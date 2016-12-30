#include "msg_passing_thread.h"


namespace MZFRAME {


msg_passing_thread::msg_passing_thread(){}

msg_passing_thread::~msg_passing_thread(){}
	
	

int msg_passing_thread::register_thread(common_thread *thread)
{
	pthread_t tid = 0;
	if (!thread) {
		return -1;
	}
	
	tid = thread->get_thread_id();
	it = _thd_fd_map.find(tid);
	if (it != _thd_fd_map.end()){
		return -1;
	}
	
	int fd[2], ret;
	
	ret = socketpair(AF_UNIX, 0, SOCK_STREAM, fd);
	ASSERT_DO(ret != -1, LOG_WARNING("socketpair fail errstr[%s]", strerror(errno)));
		
	_thd_fd_map[get_thread_id()] = fd[0];
	

    struct sockaddr_in sa;
    if(!getpeername(sockfd, (struct sockaddr *)&sa, &len))
    {
    
    }

	struct epoll_event fds;
	fds.data.fd = fd;
	fds.events = EPOLLIN;
	_net_container->add_event(fds);
	
	return 0;
}


void* msg_passing_thread::run()
{
	while (get_run_flag())
	{
		
		_net_container->obj_process();
		
		/**
		
		  业务转发
		*/
	}
	
}

NET_OBJ * gen_connect(const int fd, const sockaddr_in &addr)
{

}






}
