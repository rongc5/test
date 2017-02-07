#ifndef __LISTEN_THREAD_H__
#define __LISTEN_THREAD_H__

#include "base_def.h"
#include "common_thread.h"
#include "base_net_container.h"
#include "common_obj_container.h"
#include "listen_connect.h"
#include "common_def.h"
#include "listen_data_process.h"
#include "base_thread_mgr.h"


class listen_thread:public common_thread
{
    public:
        listen_thread():_base_container(NULL){
        };
        virtual ~listen_thread(){
            if (_base_container){
                delete _base_container;
            }
        };

        virtual void obj_process()
        {
            _base_container->obj_process();
        }

        virtual void set_channelid(int fd)
        {
            common_thread::set_channelid(fd);
        }

        void init(const string &ip, unsigned short port)
        {
            _ip = ip;
            _port = port;
            _base_container = new common_obj_container();
            listen_connect<listen_data_process> * p_connect = new listen_connect<listen_data_process>();
            listen_data_process * data_process = new listen_data_process(p_connect);
            p_connect->set_id(gen_id_str());
            p_connect->init(ip, port);
            data_process->set_thread(this);
            p_connect->set_process(data_process);
            p_connect->set_net_container(_base_container);
            set_passing_type(PASSING_ACCEPT_NONE);
        }

        uint32_t get_worker_id()
        {
            return _worker_mgr->get_idle_worker()->get_thread_index();
        }

        void set_worker_mgr(base_thread_mgr * worker_mgr)
        {
            if (_worker_mgr && _worker_mgr != worker_mgr){
                delete _worker_mgr;
            }

            _worker_mgr = worker_mgr;
        }

    protected:
        const ObjId & gen_id_str()
        {
            uint32_t thread_index = get_thread_index();
            _id_str.set_thread_index(thread_index);
            uint32_t  obj_id = _id_str.obj_id();
            obj_id++;
            _id_str.set_obj_id(obj_id);
            return _id_str;
        }

    protected:
        string _ip;
        unsigned short _port;
        base_thread_mgr *_worker_mgr;
        base_net_container * _base_container;
        ObjId _id_str;
};

#endif
