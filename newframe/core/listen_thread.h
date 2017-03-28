#ifndef __LISTEN_THREAD_H__
#define __LISTEN_THREAD_H__

#include "base_net_thread.h"
#include "common_def.h"
#include "listen_data_process.h"


template<class MSG_PROCESS>
class listen_thread: public base_net_thread
{
    public:
        listen_thread():_current_indx(0){
        };
        virtual ~listen_thread(){
        };

        void init(const string &ip, unsigned short port)
        {
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

            _fd = socket(AF_INET, SOCK_STREAM, 0);
            if (_fd < 0) 
            {
                THROW_COMMON_EXCEPT("socket error " << strerror(errno));     
            }
            setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, (void*)(&(reuse_addr)), sizeof(reuse_addr));

            if (::bind(_fd, (struct sockaddr *) &address, sizeof(address)) < 0) 
            {        
                THROW_COMMON_EXCEPT("bind error "  << strerror(errno) << " " << ip << ":" << port);
            }        

            ret = listen(_fd, 250);
            if (ret == -1)
            {
                THROW_COMMON_EXCEPT("listen error "  << strerror(errno));
            }

            set_unblock(_fd);

            event_set(&ev_accept, listen_fd, EV_READ|EV_PERSIST, on_accept, this);
            event_dispatch();
        }

        int add_worker_thread(base_net_thread * thread)
        {
            _worker_thrds.push_back(thread);

            return 0;
        }

        static void on_accept(int fd, short ev, void *arg)
        {
            listen_thread * l_thread = (listen_thread *)arg;

            if (!_worker_thrds.size()) {
                LOG_DEBUG("recv_fd: %d\n", r_msg->fd);
                return ;
            }


            {
                uint32_t index = _current_indx;
                _current_indx++;

                if (_current_indx >= _worker_thrds.size()){
                    _current_indx = 0;
                }

                _worker_thrds[index]->put_msg(fd);
            }
        }


    protected:

        string _ip;
        unsigned short _port;
        listen_connect<listen_data_process> *_listen_connect;
        vector<base_net_thread * > _worker_thrds;
        uint32_t _current_indx;
};

#endif
