#include "common_epoll.h"

int common_epoll::_s_epoll_size = 1000;
int common_epoll::_s_epoll_wait_time = 20;


void common_epoll::add_to_epoll(base_net_obj *p_obj)
{		
	int tmpOprate = EPOLL_CTL_ADD;
	struct epoll_event tmpEvent;
	memset(&tmpEvent, 0, sizeof(epoll_event));
	tmpEvent.events = p_obj->get_event();
	tmpEvent.data.ptr = p_obj;
	int ret = epoll_ctl(_epoll_fd, tmpOprate, p_obj->get_sock(), &tmpEvent);
	if (ret != 0)
		THROW_COMMON_EXCEPT(errno,  "add to epoll fail " << strerror(errno));
}

void common_epoll::del_from_epoll(base_net_obj *p_obj)
{	
	int tmpOprate = EPOLL_CTL_DEL;
	struct epoll_event tmpEvent;
	memset(&tmpEvent, 0, sizeof(epoll_event));
	tmpEvent.data.ptr = p_obj;
	int ret = epoll_ctl(_epoll_fd, tmpOprate, p_obj->get_sock(), &tmpEvent);
	if (ret != 0)
		THROW_COMMON_EXCEPT(errno,  "del from epoll fail " << strerror(errno));
}

void common_epoll::mod_from_epoll(base_net_obj *p_obj)
{
	int tmpOprate = EPOLL_CTL_MOD;
	struct epoll_event tmpEvent;
	tmpEvent.events =  p_obj->get_event();  
	tmpEvent.data.ptr = p_obj;
	int ret = epoll_ctl(_epoll_fd, tmpOprate, p_obj->get_sock(), &tmpEvent);
	if (ret != 0)
		THROW_COMMON_EXCEPT(errno,  "mod from epoll fail "<< strerror(errno));
}

int common_epoll::epoll_wait(map<uint64_t, base_net_obj*> &expect_list)
{
	int  nfds = ::epoll_wait(_epoll_fd, _epoll_events, _real_epoll_size,  _epoll_wait_time);
	if (nfds == -1)
	{
		THROW_COMMON_EXCEPT(errno,  "epoll_wait fail "<< strerror(errno));          
	}      

	for (int i =0; i < nfds; i++)
	{		
		if (_epoll_events[i].data.ptr != NULL) 		
		{
			base_net_obj * p = (base_net_obj*)(_epoll_events[i].data.ptr);
			if (p != NULL)
			{
				try
				{
					p->event_process(_epoll_events[i].events);
				}
				catch(sock_exception &e)
				{
					expect_list.insert(make_pair(p->get_id(), p));
				}
				catch(CMyCommonException &e)
				{
					expect_list.insert(make_pair(p->get_id(), p));
				}
				catch(std::exception &e)
				{
					expect_list.insert(make_pair(p->get_id(), p));
				}
			}
		}
	}
	return nfds;
}

