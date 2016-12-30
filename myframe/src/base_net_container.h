#ifndef __BASE_NET_CONTAINER_H_
#define __BASE_NET_CONTAINER_H_
#include "common_epoll.h"
class base_net_container
{
	public:
		base_net_container()
		{
			_p_epoll = NULL;
		}
		virtual ~base_net_container()
		{
		      if (_p_epoll != NULL)
                        delete _p_epoll;
		}
		
		virtual void push_net_obj(base_net_obj *p_obj) = 0;
		
		common_epoll *get_epoll()
		{
			return _p_epoll;
		}
		
		void init(const uint32_t epoll_size = 0)
		{
			_p_epoll = new common_epoll();
			_p_epoll->init(epoll_size);
		}
	protected:
		
		common_epoll *_p_epoll;
};
#endif

