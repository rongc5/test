#ifndef __LISTEN_THREAD_H__
#define __LISTEN_THREAD_H__

#include "base_net_thread.h"
#include "common_def.h"
#include "listen_connect.h"

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

            listen_connect * l_conn = listen_connect::gen_connect(ip, _port, this);
        }

        int add_worker_thread(base_net_thread * thread)
        {
            _worker_thrds.push_back(thread);

            return 0;
        }


    protected:
        void dispatch(int fd)
        {

            if (!_worker_thrds.size()) {
                LOG_DEBUG("recv_fd: %d\n", fd);
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

        vector<base_net_thread * > _worker_thrds;
        uint32_t _current_indx;
};

#endif
