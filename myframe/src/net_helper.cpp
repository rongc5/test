#include "net_helper.h"

namespace MZFRAME {
	
	
	void net_helper::event_process(epoll_event * event)
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
	
	
	ssize_t net_helper::RECV(int fd, void *buf, size_t len)
		{
			ssize_t ret = recv(fd, buf, len, MSG_DONTWAIT);
			if (ret == 0)
			{
				_process->peer_close();
				THROW_SOCKET_EXPECT(0, "the client close the socket(" << _sock << ")");
			}
			else if (ret < 0)
			{
				if (errno == EAGAIN)
				{
					if ((_epoll_event & EPOLLET) == EPOLLET) //边缘触发重新加入
					{
						_net_event = _net_event & (~BASE_READ_EVENT);
						epoll_event event;
						event.data.fd = fd;
						event.events = 
						add_event(EPOLLIN);
						_epoll->add_to_epoll()
	          ret = 0;
					}
				}
				else
				{
					THROW_SOCKET_EXPECT(errno, "this socket occur fatal error " << strerror(errno));
				}
			}
			else // > 0
			{
			}
			return ret;
		}
		
		
		
	virtual ssize_t SEND(int fd, const void *buf, const size_t len)
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