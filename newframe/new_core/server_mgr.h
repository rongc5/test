#ifndef __SERVER_MGR_H__
#define __SERVER_MGR_H__

#include "base_connect.h"
#include "listen_thread.h"
#include "base_net_thread.h"


class server_mgr
{
    public:
        server_mgr():_server_ip(""), _server_port(0)
        {
        }

        server_mgr(const string &ip, const unsigned short port)
        {
            init(ip, port);
        }

        virtual ~server_mgr()
        {

        }

        void init(const string &ip, const unsigned short port)
        {
            _server_ip = ip;
            _server_port = port;

            _listen_obj_thread = new listen_thread();
            _listen_obj_thread->init(_server_ip, _server_port);
        }

        int add_worker_thread(base_net_thread * thread)
        {
           if (_listen_obj_thread && thread){ 
               _listen_obj_thread->add_worker_thread(thread);
               thread->start();
           }

            return 0;
        }

        void run()
        {
            _listen_obj_thread->start();
        }

    private:
        listen_thread * _listen_obj_thread;
        string _server_ip;
        unsigned short _server_port;
};

#endif

