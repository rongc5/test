#ifndef __LOG_THREAD_H__
#define __LOG_THREAD_H__

#include "common_def.h"
#include "base_thread.h"


class log_thread : public base_thread
{
    public:
        log_thread():_net_container(NULL)
    {}

        virtual ~log_thread()
        {
            if (_net_container){
                delete _net_container;
            }
        }
        
        static int add(int fd)
        {
            log_thread * thread = base_singleton<log_thread>::get_instance();
            if (!thread) {
                thread = new log_thread()
                base_singleton<log_thread>::::set_instance(thread);
                thread->start();
            }

            thread->gen_connect(fd, EPOLL_LT_TYPE);
        }

        virtual void* run()
        {
            while (get_run_flag())
            {
                _net_container->obj_process();
            }
        }

        NET_OBJ * gen_connect(const int fd, EPOLL_TYPE epoll_type)
        {
            NET_OBJ * p_connect = NULL;
            p_connect = new base_connect<log_msg_process>(fd, epoll_type);
            log_msg_process *process = new log_msg_process((NET_OBJ*)p_connect);

            base_connect<log_msg_process> * tmp_con =                               (base_connect<log_msg_process> *)p_connect;
            tmp_con->set_process(process);

            return p_connect;
        }

    protected:
        base_net_container * _net_container;        
};


#endif


