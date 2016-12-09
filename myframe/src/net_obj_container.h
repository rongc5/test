#ifndef __NET_OBJ_CONTAINER_H__
#define __NET_OBJ_CONTAINER_H__

namespace MZFRAME {
	
	enum OBJ_ACTION {
		  OBJ_UNKNOWN,
			OBJ_SEND,
			OBJ_RECV		
	};
	
	struct net_obj {
		int _fd;
		string _buf;//buf
		int _buf_len; //buf 长度
		int _len;//已经发送或者接收的长度
		uint64_t _begin_time;//开始处理时间
		OBJ_ACTION _obj_action;
		
		net_obj()
		{
				_fd = 0;
				_buf_len = 0;
				_len = 0;
				_begin_time = 0;
				_obj_action = OBJ_UNKNOWN;
		}
	};
	
	
	class net_obj_container {
		
		public:
			net_obj_container();
			virtual ~net_obj_container();
			
			void init();
			
			virtual void event_process(epoll_event * events);
			virtual ssize_t RECV(epoll_event * events, void *buf, size_t len);
			virtual ssize_t SEND(epoll_event * events, const void *buf, const size_t len);
			virtual void recv_process();
			virtual void send_process();
			
			virtual void obj_process();
			
			void put_net_obj(net_obj &p_obj);
			
			void set_epoll_size(uint32_t _epoll_size);
			void set_epoll_time(int epoll_time);
			
		protected:
			
			map<int, net_obj> _recv_list; //已接收队列
			map<int, net_obj> _send_list; //待发送队列
			epoll_helper * _epoll;
			uint32_t _epoll_size; // 
			int epoll_time; // 阻塞等待时间， <0 阻塞模式， 
	};
	
	
	
}


#endif