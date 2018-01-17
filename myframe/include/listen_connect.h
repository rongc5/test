#ifndef __LISTEN_CONNECT_H__
#define __LISTEN_CONNECT_H__

#include "common_def.h"
#include "log_helper.h"
#include "base_net_obj.h"


template<class PROCESS>
class listen_connect:public base_net_obj
{
    public:
        listen_connect(const string &ip, unsigned short port)
    {
        _process = NULL;
          _ip = ip;
            _port = port;


            struct sockaddr_in address; 
            int reuse_addr = 1;  

            memset((char *) &address, 0, sizeof(address)); 
            address.sin_family = AF_INET;
            address.sin_port = htons(port); 
            int ret = 0;         

            if (ip != "")        
            {
                inet_aton(ip.c_str(), &address.sin_addr);
            }
            else
            {
                address.sin_addr.s_addr = htonl(INADDR_ANY); 
            }

            int fd = socket(AF_INET, SOCK_STREAM, 0);
            if (fd < 0)         
            {
                THROW_COMMON_EXCEPT("socket error " << strerror(errno));     
            }
            setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void*)(&(reuse_addr)), sizeof(reuse_addr)); 

            if (::bind(fd, (struct sockaddr *) &address, sizeof(address)) < 0) 
            {        
                THROW_COMMON_EXCEPT("bind error "  << strerror(errno) << " " << ip << ":" << port);
            }        

            ret = listen(fd, 250);
            if (ret == -1)
            {
                THROW_COMMON_EXCEPT("listen error "  << strerror(errno));
            }

            set_unblock(fd);
            base_net_obj(fd);
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
    	  string _ip;
        unsigned short _port;
        PROCESS *_process;
};


#endif

