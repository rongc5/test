#ifndef __LISTEN_DATA_PROCESS_H__
#define __LISTEN_DATA_PROCESS_H__

#include "common_def.h"

class base_listen_thread;
class listen_connect;
class base_net_obj;
class listen_data_process
{
    public:
        listen_data_process(void *p);

        virtual ~listen_data_process(){}

        void process(int fd);

        void add_worker_thread(uint32_t thread_index);

        void set_listen_thread(base_listen_thread * thread);

        base_listen_thread * get_listen_thread();

    protected:	
        listen_connect * _p_connect;
        base_listen_thread * _listen_thread;
        vector<uint32_t> _worker_thd_vec;
};

#endif

