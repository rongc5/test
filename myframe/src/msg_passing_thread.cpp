#include "msg_passing_thread.h"


namespace MZFRAME {


msg_passing_thread::msg_passing_thread():_net_container(NULL){}

msg_passing_thread::~msg_passing_thread(){
    if (_net_container){
        delete _net_container;
    }
}
	
	

int msg_passing_thread::register_thread(common_thread *thread)
{
	pthread_t tid = 0;
	if (!thread) {
		return -1;
	}
	
    if (!_net_container){
        _net_container = new (std::nothrow)common_obj_container();
        LOG_WARNING("_net_container NULL");
        return -1;
    }


	tid = thread->get_thread_id();
	it = _thd_channel_map.find(tid);
	if (it != _thd_channel_map.end()){
		return -1;
	}
	
	int fd[2], ret;
	
	ret = socketpair(AF_UNIX, 0, SOCK_STREAM, fd);
	ASSERT_DO(ret != -1, LOG_WARNING("socketpair fail errstr[%s]", strerror(errno)));
		

    struct sockaddr_in sa;
    if(!getpeername(fd[0], (struct sockaddr *)&sa, &len))
    {
         LOG_WARNING("getpeername fail errstr[%s]", strerror(errno)); 
    }

    NET_OBJ *p_connect = gen_connect(fd[0], sa);
    if (p_connect){
        _thd_fd_map[get_thread_id()] = fd[0];
        _net_container->push_net_obj(p_connect);
        thread->set_channelid(fd[1]);
    }
	
	return 0;
}


void* msg_passing_thread::run()
{
	while (get_run_flag())
	{
		_net_container->obj_process();
	}

    return NULL;
}

NET_OBJ * gen_connect(const int fd, const sockaddr_in &addr)
{

}






}
