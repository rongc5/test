#ifndef __NET__HELPER_H__
#define __NET__HELPER_H__

#include "def.h"

namespace MZFRAME {
	


	
	
	template<class PROCESS>
	class net_helper{
		public:
			net_helper();
			virtual ~net_helper();
			
			virtual void event_process(const int32_t events);
			virtual ssize_t RECV(void *buf, size_t len);
			virtual ssize_t SEND(const void *buf, const size_t len);
			
			
		protected:
			map<int, >
			epoll_helper * _epoll;

	};
	
	
	
	
}



#endif