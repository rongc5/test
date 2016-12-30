#ifndef __COMMON_EPOLL_H_
#define __COMMON_EPOLL_H_

#include <sys/epoll.h>
#include "net_obj.h"

using namespace MYFRAMECOMMON;
using namespace std;

class common_epoll
{
    public:
        common_epoll()
        {
            _epoll_events = NULL;
            _real_epoll_size = 0;
        }

        ~common_epoll()
        {
            if (_epoll_events != NULL)
                delete [] _epoll_events;
        }

        void init(const uint32_t epoll_size = 0, int epoll_wait_time)
        {
            _epoll_size = (epoll_size == 0)?DAFAULT_EPOLL_SIZE:epoll_size;
            
            _epoll_wait_time = epoll_wait_time;
            	
            _epoll_fd = epoll_create(_epoll_size);
            if (_epoll_fd == -1)
            {       
                THROW_COMMON_EXCEPT(errno, "epoll_create fail " << strerror(errno));
            }
            _epoll_events = new epoll_event[_epoll_size]; 
                            
        }

        void add_to_epoll(base_net_obj *p_obj);

        void del_from_epoll(base_net_obj *p_obj);

        void mod_from_epoll(base_net_obj *p_obj);

        int epoll_wait(map<uint64_t, base_net_obj*> &expect_list);

    private:
        int _epoll_fd;
        struct epoll_event *_epoll_events;
        uint32_t _epoll_size;
        int _epoll_wait_time;
};

#endif

