#ifndef __SERVER_MGR_H__
#define __SERVER_MGR_H__

#include "base_connect.h"
#include "listen_thread.h"
#include "base_thread_mgr.h"
#include "common_net_thread.h"
#include "passing_msg_thread.h"


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
            if (!num) {
                _thread_num = 1;
            }
            _server_ip = ip;
            _server_port = port;
        }

        void run()
        {
            _worker_thread = new common_net_thread<MSG_PROCESS>[_thread_num];

            base_thread_mgr * thread_mgr = new base_thread_mgr();

            for (int i = 0; i < _thread_num; i++) {
                _worker_thread[i].start();
                thread_mgr->add_thread(&_worker_thread[i]);
                passing_msg_thread::register_thread(&_worker_thread[i]);
            }

            _listen_obj_thread = new listen_thread();
            _listen_obj_thread->init(_server_ip, _server_port);
            _listen_obj_thread->set_worker_mgr(thread_mgr);
            passing_msg_thread::register_thread(_listen_obj_thread);
            //thread_mgr->start();
            _listen_obj_thread->start();
        }

    private:
        listen_thread * _listen_obj_thread;
        common_net_thread<MSG_PROCESS> *_worker_thread;
        int _thread_num;
        string _server_ip;
        unsigned short _server_port;
};

#endif

