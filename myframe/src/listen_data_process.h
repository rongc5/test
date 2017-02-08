#ifndef __LISTEN_DATA_PROCESS_H__
#define __LISTEN_DATA_PROCESS_H__

#include "common_def.h"


class listen_thread;
class base_net_obj;
class listen_data_process
{
    public:
        listen_data_process(void *p);

        virtual ~listen_data_process(){}

        virtual size_t process(int fd);

        void set_thread(listen_thread * thread);

    protected:	
        base_net_obj * _p_connect;
        listen_thread * _thread;
};

#endif

