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
		ASSERT_DO(!_epoll_events, delete [] _epoll_events);
    }

    void epoll_helper::init(const uint32_t epoll_size, uint32_t epoll_wait_time)
    {
        _real_epoll_size = (epoll_size == 0)?_s_epoll_size:epoll_size;
        _epoll_fd = epoll_create(_real_epoll_size);
		_real_epoll_wait_time = epoll_wait_time;
		
		ASSERT_DO(_epoll_fd != -1, LOG_WARNING("epoll_create fail errno[%u] errstr[%s]", errno, strerror(errno)));
    }
	
	int common_epoll::epoll_wait()
	{
		int wait_time = _real_epoll_wait_time;
		if (num > 0)
		{
			wait_time = 0;
		}

		int  nfds = ::epoll_wait(_epoll_fd, _epoll_events, _real_epoll_size,  wait_time);
		
		ASSERT_DO(nfds != -1, LOG_WARNING("epoll_wait fail errno[%u] errstr[%s]", errno, strerror(errno)));

		for (int i =0; i < nfds; i++)
		{		
			if (_epoll_events[i].data.ptr == NULL)
			{
				LOG_WARNING("_epoll_events[i].data.ptr is NULL");
				continue;
			}
			else		
			{
				base_obj * p = dynamic_cast<base_obj*>(_epoll_events[i].data.ptr);
				if (p != NULL)
				{
					try
					{
						p->event_process(_epoll_events[i].events);
					}
					catch(std::exception &e)
					{
						LOG_WARNING("exception [%s]", e.what());
					}
				}
				else 
				{
					LOG_WARNING("_epoll_events[i].data.ptr is not base_net_obj");
				}
			}
		}
		return nfds;
	}





}





