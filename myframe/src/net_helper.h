#ifndef __NET__HELPER_H__
#define __NET__HELPER_H__

#include "def.h"

namespace MZFRAME {
	
	template<class PROCESS>
	class net_helper{
		public:
			net_helper();
			virtual ~net_helper();
			
			virtual void event_process(epoll_event * events);
			virtual ssize_t RECV(epoll_event * events, void *buf, size_t len);
			virtual ssize_t SEND(epoll_event * events, const void *buf, const size_t len);
			virtual void recv_process();
			virtual void send_process();
			
			
			
			
		protected:
					
			
		protected:
			map<int, net_obj> _recv_list; //接收队列
			map<int, net_obj> _send_list; //发送队列
			epoll_helper * _epoll;
	};
	
	
	
	
}



#endif