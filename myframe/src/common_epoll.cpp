#include "common_epoll.h"
#include "net_obj.h"
#include "common_exception.h"


void common_epoll::add_to_epoll(base_net_obj *p_obj)
{		
    int tmpOprate = EPOLL_CTL_ADD;
    struct epoll_event tmpEvent;
    memset(&tmpEvent, 0, sizeof(epoll_event));
    tmpEvent.events = p_obj->get_event();
    tmpEvent.data.ptr = p_obj;
    int ret = epoll_ctl(_epoll_fd, tmpOprate, p_obj->get_sock(), &tmpEvent);
    PDEBUG("add to epoll _epoll_fd[%d] _get_sock [%d]\n", _epoll_fd, p_obj->get_sock());
    if (ret != 0) {
        PDEBUG("add to epoll fail %s", strerror(errno));
        THROW_COMMON_EXCEPT("add to epoll fail " << strerror(errno));
    }
}

void common_epoll::del_from_epoll(base_net_obj *p_obj)
{	
    int tmpOprate = EPOLL_CTL_DEL;
    struct epoll_event tmpEvent;
    memset(&tmpEvent, 0, sizeof(epoll_event));
    tmpEvent.data.ptr = p_obj;
    int ret = epoll_ctl(_epoll_fd, tmpOprate, p_obj->get_sock(), &tmpEvent);
    if (ret != 0)
        THROW_COMMON_EXCEPT("del from epoll fail " << strerror(errno));
}

void common_epoll::mod_from_epoll(base_net_obj *p_obj)
{
    int tmpOprate = EPOLL_CTL_MOD;
    struct epoll_event tmpEvent;
    tmpEvent.events =  p_obj->get_event();  
    tmpEvent.data.ptr = p_obj;
    int ret = epoll_ctl(_epoll_fd, tmpOprate, p_obj->get_sock(), &tmpEvent);
    if (ret != 0)
        THROW_COMMON_EXCEPT("mod from epoll fail "<< strerror(errno));
}

int common_epoll::epoll_wait(map<ObjId, base_net_obj*> &expect_list)
{
    int  nfds = ::epoll_wait(_epoll_fd, _epoll_events, _epoll_size,  _epoll_wait_time);
    if (nfds == -1)
    {
        PDEBUG("epoll_wait fail [%s]\n", strerror(errno));          
        THROW_COMMON_EXCEPT("epoll_wait fail "<< strerror(errno));          
    }      

    for (int i =0; i < nfds; i++)
    {		
        PDEBUG("_epoll_fd [%d]\n", _epoll_fd);
        if (_epoll_events[i].data.ptr != NULL) 		
        {
            base_net_obj * p = (base_net_obj*)(_epoll_events[i].data.ptr);
            if (p != NULL)
            {
                try
                {
                    p->event_process(_epoll_events[i].events);
                    PDEBUG("get_sock[%d]\n", p->get_sock());
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
