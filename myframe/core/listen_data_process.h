#ifndef __LISTEN_DATA_PROCESS_H__
#define __LISTEN_DATA_PROCESS_H__

#include "common_def.h"
#include "base_net_thread.h"
#include "net_obj.h"

class listen_data_process
{
    public:
        listen_data_process(void *p);

        virtual ~listen_data_process(){}

        size_t process(int fd);

        void set_thread(base_net_thread * thread);

    protected:	
        base_net_obj * _p_connect;
        base_net_thread * _thread;
};

#endif

