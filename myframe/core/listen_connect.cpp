#include "common_def.h"
#include "base_net_obj.h"
#include "listen_connect.h"
#include "listen_data_process.h"
#include "log_helper.h"


listen_connect::listen_connect(int fd):base_net_obj(fd)
{
    _process = NULL;
}

listen_connect::~listen_connect()
{	
    if (_process)
        delete _process;
}

void listen_connect::event_process(int events)
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

void listen_connect::set_process(listen_data_process *p)
{
    if (_process != NULL)
        delete _process;
    _process = p;
}

listen_data_process * listen_connect::get_process()
{
    return _process;
}


