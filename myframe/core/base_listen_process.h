#ifndef __BASE_LISTEN_PROCESS_H__
#define __BASE_LISTEN_PROCESS_H__

#include "common_def.h"
#include "net_obj.h"

class base_listen_connect;
class listen_data_process
{
    public:
        listen_data_process(void *p);

        virtual ~listen_data_process(){}

        virtual size_t process(int fd);

    protected:	
        base_listen_connect * _p_connect;
};

#endif

