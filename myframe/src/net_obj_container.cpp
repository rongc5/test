#include "net_obj_container.h"

namespace MZFRAME {

void net_obj_container::init()
	{
			_p_epoll = new common_epoll();
			_p_epoll->init(epoll_size, epoll_time);
	}


void net_obj_container::put_net_obj(net_obj &p_obj)
	{
		map<int, net_obj>::iterator it;
			
		map<int, net_obj> * dst_list = NULL;
		
		if (p_obj->_obj_action == OBJ_SEND) {
					dst_list =  & _send_list;
		}else if (p_obj->_obj_action == OBJ_RECV) {
					dst_list =  & _recv_list;	
		}else {
				return ;	
		}
		
		dst_list[p_obj._fd] = p_obj;		 
	}
	
	void net_obj_container::obj_process()
		{
				_epoll->epoll_wait();
		}
	
	
		void net_obj_container::event_process(epoll_event * event)
		{
				if ((event->events & EPOLLERR) == EPOLLERR || (event->events & EPOLLHUP) == EPOLLHUP)
				{
					THROW_SOCKET_EXPECT(0, "epoll error ");
				}
		

				if ((event->events & EPOLLIN) == EPOLLIN) //读
				{
					recv_process();
				}
		
				if ((event->events & EPOLLOUT) == EPOLLOUT ) //写
				{
					send_process();
				}		
		}
	
	
	
  ssize_t SEND(int fd, const void *buf, const size_t len)
	{
		if (len == 0) //上层抛一个长度为0的数据过来 ,直接关闭
		{
			THROW_SOCKET_EXPECT(0, "close the socket " << _sock);
		}
		
		ssize_t ret =  send(_sock, buf, len, MSG_DONTWAIT);
		if (ret < 0)
		{
			if (errno != EAGAIN && errno != EWOULDBLOCK)
			{
				THROW_SOCKET_EXPECT(errno, "send data error " << strerror(errno));
			}
			else
			{
				if ((_epoll_event & EPOLLET) == EPOLLET) //边缘触发重新加入
				{
					_net_event = _net_event & (~BASE_WRITE_EVENT);
					add_event(EPOLLOUT);
				}
			}
		}
		return ret;
	}














}