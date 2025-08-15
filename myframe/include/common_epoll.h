#ifndef __COMMON_EPOLL_H_
#define __COMMON_EPOLL_H_

#include "common_def.h"
#include "common_exception.h"
#include "common_util.h"

class base_net_obj;
class common_epoll
{
    public:
        common_epoll()
        {
            _epoll_events = NULL;
            _epoll_size = 0;
        }

        ~common_epoll()
        {
            if (_epoll_events != NULL)
                delete [] _epoll_events;
        }

        void init(const uint32_t epoll_size = DAFAULT_EPOLL_SIZE, int epoll_wait_time = DEFAULT_EPOLL_WAITE)
        {
            _epoll_size = (epoll_size == 0)?DAFAULT_EPOLL_SIZE:epoll_size;

            _epoll_wait_time = epoll_wait_time;

            _epoll_fd = epoll_create(_epoll_size);
            if (_epoll_fd == -1)
            {       
                std::string err = strError(errno);
                THROW_COMMON_EXCEPT("epoll_create fail " << err.c_str());
            }
            _epoll_events = new epoll_event[_epoll_size]; 

        }

        void add_to_epoll(base_net_obj * p_obj);

        void del_from_epoll(base_net_obj * p_obj);

        void mod_from_epoll(base_net_obj * p_obj);

        int epoll_wait(std::map<ObjId, std::shared_ptr<base_net_obj> > &expect_list, std::map<ObjId, std::shared_ptr<base_net_obj> > &remove_list, uint32_t num);

    private:
        int _epoll_fd;
        struct epoll_event *_epoll_events;
        uint32_t _epoll_size;
        int _epoll_wait_time;
};



#endif

