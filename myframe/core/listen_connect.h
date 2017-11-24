#ifndef __LISTEN_CONNECT_H__
#define __LISTEN_CONNECT_H__

#include "common_def.h"
#include "log_helper.h"
#include "base_net_obj.h"


template<class PROCESS>
class listen_connect:public base_net_obj
{
    public:
        listen_connect(int fd):base_net_obj(fd)
    {
        _process = NULL;
    }

        virtual ~listen_connect()
        {
            if (_process)
                delete _process;
        }

        virtual void event_process(int events)
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


        virtual int real_net_process()
        {
            return 0;
        }

        void set_process(PROCESS *p)
        {
            if (_process != NULL)
                delete _process;
            _process = p;

        }


    private:
        PROCESS *_process;
};


#endif

