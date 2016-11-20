#ifndef __EPOLL_HELPER__
#define __EPOLL_HELPER__

namespace MZFRAME {

    class epoll_helper{
        
        public:
            epoll_helper();
            ~epoll_helper();
            void init(const uint32_t epoll_size = 0);

            void add_to_epoll(base_net_obj *p_obj);
            void del_from_epoll(base_net_obj *p_obj);
            void mod_from_epoll(base_net_obj *p_obj);

        private:
            int _epoll_fd;
            struct epoll_event *_epoll_events;
            uint32_t _real_epoll_size;
    };
    
}



#endif

