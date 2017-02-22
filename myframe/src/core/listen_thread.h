#ifndef __LISTEN_THREAD_H__
#define __LISTEN_THREAD_H__

#include "common_thread.h"
#include "base_net_container.h"
#include "forward_obj_container.h"
#include "listen_connect.h"
#include "common_def.h"
#include "listen_data_process.h"


class listen_thread:public common_thread
{
    public:
        listen_thread():_base_container(NULL), _current_indx(0){
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

        void init(const string &ip, unsigned short port)
        {
            _ip = ip;
            _port = port;
            _base_container = new forward_obj_container();
            listen_connect<listen_data_process> * p_connect = new listen_connect<listen_data_process>();
            listen_data_process * data_process = new listen_data_process(p_connect);
            p_connect->set_id(gen_id_str());
            p_connect->init(ip, port);
            data_process->set_thread(this);
            p_connect->set_process(data_process);
            p_connect->set_net_container(_base_container);
        }

        int add_worker_thread(common_thread * thread)
        {
            _worker_thrds.push_back(thread);
            add_forward_thread(thread, gen_id_str());

            return 0;
        }
        
        int add_forward_thread(common_thread *thread, const ObjId & id_str)
        {
            _base_container->add_forward_thread(thread, id_str);
            return 0;
        }

            
        uint32_t get_worker_id()
        {
            uint32_t index = _current_indx;
            _current_indx++;

            if (_current_indx >= _worker_thrds.size()){
                _current_indx = 0;
            }

            return _worker_thrds[index]->get_thread_index();
        }

        uint32_t get_dest_channelid(uint32_t thread_index)
        {
            return _base_container->get_dest_channelid(thread_index);
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
        forward_obj_container * _base_container;
        ObjId _id_str;
        vector<base_thread*> _worker_thrds;
        uint32_t _current_indx;
};

#endif
