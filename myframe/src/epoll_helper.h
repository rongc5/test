#ifndef __EPOLL_HELPER__
#define __EPOLL_HELPER__

#include "def.h"


namespace MZFRAME {
	
#define DEFAULT_EPOLL_SIZE 1000
#define DEFAULT_EPOLL_TIME 20
	
    class epoll_helper{

        public:
            epoll_helper();
            ~epoll_helper();
            void init(const uint32_t epoll_size, const int epoll_wait_time);

            void add_to_epoll(epoll_event & event);
            void del_from_epoll(epoll_event & event);
            void mod_from_epoll(epoll_event & event);			
            int epoll_wait();
            
            void set_container(net_obj_container * net_container);

        private:
        	  net_obj_container * _net_process;
            int _epoll_fd;
            struct epoll_event *_epoll_events;
            uint32_t _real_epoll_size;
            uint32_t _real_epoll_wait_time;
    };

}



#endif

