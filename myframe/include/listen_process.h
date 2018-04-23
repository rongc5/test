#ifndef __LISTEN_PROCESS_H__
#define __LISTEN_PROCESS_H__

#include "common_def.h"
#include "base_net_thread.h"
#include "base_net_obj.h"

template<class PROCESS>
class listen_process
{
    public:
        listen_process(base_net_obj *p)
        {
            _listen_thread = NULL;
            _p_connect = p;
        }

        ~listen_process(){}

        void process(int fd)
        {
            std::shared_ptr<base_net_obj>  net_obj(PROCESS::gen_listen_obj(fd));

            ObjId id;
            if (_worker_thd_vec.size()) {
                int index = (unsigned long) net_obj % _worker_thd_vec.size();
                id._thread_index = _worker_thd_vec[index]; 
            } else {
                id._thread_index = _listen_thread->get_thread_index(); 
            }
            std::shared_ptr<normal_msg> ng = std::static_pointer_cast<normal_msg>(net_obj);
            base_net_thread::put_obj_msg(id, ng);		
        }

        void add_worker_thread(uint32_t thread_index)
        {
            _worker_thd_vec.push_back(thread_index);
        }

        void set_listen_thread(base_net_thread * thread)
        {
            _listen_thread = thread;
        }

        base_net_thread * get_listen_thread()
        {
            return _listen_thread;
        }

    protected:	
        base_net_obj * _p_connect;
        base_net_thread * _listen_thread;
        std::vector<uint32_t> _worker_thd_vec;
};

#endif

