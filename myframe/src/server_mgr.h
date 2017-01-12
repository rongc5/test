#ifndef __SERVER_MGR_H__
#define __SERVER_MGR_H__

#include "base_connect.h"
#include "listen_thread.h"
#include "net_thread_mgr.h"

using namespace MZFRAME;

template<class MSG_PROCESS>
class server_mgr
{
    public:
        server_mgr():_thread_num(0), _server_ip(""), _server_port(0)
        {
        }

        server_mgr(const int num, const string &ip, const unsigned short port)
        {
            init(num, ip, port);
        }

        virtual ~server_mgr()
        {
        }

        void init(const int num, const string &ip, const unsigned short port)
        {
            _thread_num = num;
            _server_ip = ip;
            _server_port = port;
        }

        void run()
        {
            _worker_mgr = new  net_thread_mgr<MSG_PROCESS>(_thread_num); 
            _worker_mgr.start();

            _listen_obj_thread = new listen_thread<net_thread_mgr<MSG_PROCESS> >();
            _listen_obj_thread->init(_server_ip, _server_port);
            _listen_obj_thread->set_worker_mgr(_worker_mgr);
            _listen_obj_thread->start();
        }

    private:
        listen_thread<net_thread_mgr<MSG_PROCESS> > * _listen_obj_thread;
        net_thread_mgr<MSG_PROCESS> *_worker_mgr;
        int _thread_num;
        string _server_ip;
        unsigned short _server_port;
};

#endif

