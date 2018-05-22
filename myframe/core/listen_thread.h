#ifndef __LISTEN_THREAD_H__
#define __LISTEN_THREAD_H__

#include "base_net_thread.h"
#include "common_obj_container.h"
#include "listen_connect.h"
#include "common_def.h"
#include "listen_process.h"


class listen_thread: public base_net_thread
{
    public:
        listen_thread():_listen_connect(NULL)
    {
    }

        virtual ~listen_thread()
        {
        }

        void init(const std::string &ip, unsigned short port)
        { 
            _ip = ip;
            _port = port;
            std::shared_ptr<listen_connect<listen_process> > p(new listen_connect<listen_process>(ip, port));
            _listen_connect = p;
            _process =  new listen_process(_listen_connect);
            _process->set_listen_thread(this);
            _listen_connect->set_process(_process);
            _listen_connect->set_net_container(_base_container);
        }

        int add_worker_thread(uint32_t thread_index)
        {
            _process->add_worker_thread(thread_index);
        }

    protected:

        std::string _ip;
        unsigned short _port;
        std::shared_ptr<listen_connect<listen_process> > _listen_connect;
        listen_process * _process;
};

#endif
