#include "common_def.h"
#include "base_net_obj.h"
#include "base_listen_connect.h"
#include "base_listen_connect.h"

base_listen_connect::base_listen_connect()
{
    _port = 0;
    _process = NULL;
}

base_listen_connect::~base_listen_connect()
{	
}

void base_listen_connect::event_process(int events)
{	
    if ((events & EPOLLIN) == EPOLLIN)
    {
        int tmp_sock = 0;
        sockaddr_in addr;	
        socklen_t len = 0;
        while((tmp_sock = accept(_fd, (sockaddr*)&addr, &len)) != -1)
        {				
            LOG_DEBUG("recv fd[%d]\n", tmp_sock);
            _process->process(tmp_sock);
        }
    }
    else
    {
    }
}		

void  base_listen_connect::handle_timeout(const uint32_t timer_type)
{
    return;
}

int base_listen_connect::destroy() 
{
    delete this;
    return 0;
}

void base_listen_connect::set_process(base_listen_process *p)
{
    if (_process != NULL)
        delete _process;
    _process = p;
}

size_t base_listen_connect::process_recv_msg(normal_obj_msg * p_msg)
{
    REC_OBJ<normal_obj_msg> rc(p_msg);
}

base_listen_process * base_listen_connect::get_process()
{
    return _process;
}


