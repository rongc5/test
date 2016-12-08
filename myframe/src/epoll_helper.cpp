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
		
		ASSERT_DO(_epoll_fd != -1, LOG_WARNING("epoll_create fail errstr[%s]", strerror(errno)));
    }
    
  
void common_epoll::add_to_epoll(epoll_event & event)
{		
	int ret = epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, event.data.fd, &event);
	ASSERT_DO(ret == 0, LOG_WARNING("add to epoll fail errstr[%s]", strerror(errno)));
}
    
void common_epoll::del_from_epoll(epoll_event & event)
{	
	int ret = epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, event.data.fd, &event);
	ASSERT_DO(ret == 0, LOG_WARNING("del from epoll fail errstr[%s]", strerror(errno)));
}

void common_epoll::mod_from_epoll(epoll_event & event)
{
	int ret = epoll_ctl(_epoll_fd, EPOLL_CTL_MOD, event.data.fd, &event);
	ASSERT_DO(ret == 0, LOG_WARNING("mod from epoll fail errstr[%s]", strerror(errno)));
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
					try
					{
						_net_process->event_process(_epoll_events[i].events);
					}
					catch(std::exception &e)
					{
						LOG_WARNING("exception [%s]", e.what());
					}
		}
		return nfds;
	}





