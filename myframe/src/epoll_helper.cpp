#include "epoll_helper.h"

namespace MZFRAME {


    epoll_helper::epoll_helper()
    {
        _epoll_fd = 0;
        _epoll_events = NULL;
        _real_epoll_size = 0;
    }

    epoll_helper::~epoll_helper()
    {
        if (_epoll_events != NULL)
            delete [] _epoll_events;
    }

    void epoll_helper::init(const uint32_t epoll_size)
    {
        _real_epoll_size = (epoll_size == 0)?_s_epoll_size:epoll_size;
        _epoll_fd = epoll_create(_real_epoll_size);

        if (_epoll_fd == -1)
        {       
            LOG_WARNING("epoll_create fail errno[%u] errstr[%s]", errno, strerror(errno));
        }
    }





}





