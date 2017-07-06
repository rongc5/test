#ifndef __BASE_LISTEN_PROCESS_H__
#define __BASE_LISTEN_PROCESS_H__

#include "common_def.h"

class base_listen_connect;
class base_net_obj;
class listen_data_process
{
    public:
        listen_data_process(void *p);

        virtual ~listen_data_process(){}

        void process(int fd);

        virtual base_net_obj * gen_net_obj(int fd) = 0;

        void add_worker_thread(uint32_t thread_index);

    protected:	
        base_listen_connect * _p_connect;
        vector<uint32_t> _worker_thd_vec;
};

#endif

