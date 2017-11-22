#ifndef __LISTEN_THREAD_H__
#define __LISTEN_THREAD_H__

#include "base_listen_thread.h"
#include "base_def.h"
#include "base_connect.h"
#include "http_res_process.h"
#include "sah_data_process.h"

class listen_thread:public base_listen_thread
{
    public:

         virtual base_net_obj * gen_listen_obj(int fd)
         {
            base_connect<http_res_process> * connect = new base_connect<http_res_process>(fd);
            http_res_process * res_process = new http_res_process(connect);
            sah_data_process * sa_process = new sah_data_process(res_process);
            res_process->set_process(sa_process);
            connect->set_process(res_process);

            return connect;
         }
};


#endif
