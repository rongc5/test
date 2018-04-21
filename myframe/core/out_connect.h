#ifndef __OUT_CONNECT_H__
#define __OUT_CONNECT_H__

#include "common_def.h"
#include "base_net_obj.h"
#include "base_net_container.h"
#include "log_helper.h"
#include "common_exception.h"
#include "common_epoll.h"
#include "base_connect.h"

enum CONNECT_STATUS
{
    CLOSED = 0,
    CONNECT_OK = 1,
    CONNECTING = 2  
};


template<class PROCESS>
class out_connect:public base_connect<PROCESS>
{
    public:

        out_connect(const std::string &ip, unsigned short port)
        {
            _status = CLOSED;
            _ip = ip;
            _port = port;
        }

        virtual ~out_connect()
        {
            _status = CLOSED;

        }

        void connect()
        {
            base_net_obj::_fd = socket(AF_INET, SOCK_STREAM, 0);
            if (base_net_obj::_fd == -1)
            {
                THROW_COMMON_EXCEPT("create sock fail " << strerror(errno));
            }

            set_unblock(base_net_obj::_fd);

            sockaddr_in address;
            memset((char *) &address, 0, sizeof(address));
            address.sin_family = AF_INET;
            address.sin_port = htons(_port);
            inet_aton(_ip.c_str(), &address.sin_addr);

            int ret = ::connect(base_net_obj::_fd, (sockaddr*)&address, sizeof(address));
            if (ret != 0)
            {
                if (errno != EINPROGRESS && errno != EALREADY)
                {
                    LOG_WARNING("connect fail:%s", strerror(errno));
                    return;
                }
            }
            out_connect<PROCESS>::_epoll_event = out_connect<PROCESS>::_epoll_event | EPOLLOUT;
            _status = CONNECTING;
        }

        virtual void update_event(int event)
        {
            if (_status == CONNECT_OK)
                base_connect<PROCESS>::update_event(event);
        }

        virtual void event_process(int event)
        {
            if ((event & EPOLLERR) == EPOLLERR || (event & EPOLLHUP) == EPOLLHUP)
            {
                THROW_COMMON_EXCEPT("epoll error "<< strerror(errno));
            }

            if (_status == CONNECTING)
            {
                if ((event & EPOLLIN) == EPOLLIN && (event & EPOLLOUT) == EPOLLOUT) //
                {
                    int ret = recv(base_net_obj::_fd, NULL, 0, MSG_DONTWAIT);
                    if (ret == 0)
                    {
                        _status = CONNECT_OK;
                        connect_ok_process();
                    }
                    else
                    {
                        THROW_COMMON_EXCEPT("connect  can read and can write, error");
                    }				
                }
                else if ((event & EPOLLOUT) == EPOLLOUT) //
                {
                    LOG_DEBUG("connect ok %s:%d", _ip.c_str(), _port);
                    _status = CONNECT_OK;
                    connect_ok_process();
                }
                else if ((event & EPOLLIN) == EPOLLIN) //
                {
                    THROW_COMMON_EXCEPT("connect only can read, error");
                }
                else
                {
                    THROW_COMMON_EXCEPT("connect  error ");
                }
            }
            else 
            {
                if ((event & EPOLLIN) == EPOLLIN) //读
                {
                    base_connect<PROCESS>::real_recv();
                }

                if ((event & EPOLLOUT) == EPOLLOUT ) //写
                {
                    base_connect<PROCESS>::real_send();
                }	
            }
        }

        virtual void connect_ok_process()
        {
            LOG_DEBUG("CONNECT OK");
            return;
        }

    protected:
        std::string _ip;
        unsigned short _port;
        CONNECT_STATUS _status;
};


#endif
