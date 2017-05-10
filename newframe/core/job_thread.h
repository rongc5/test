#ifndef __JOB_THREAD_H__
#define __JOB_THREAD_H__

#include "common_def.h"
#include "base_net_thread.h"
#include "http_client_thread.h"

class base_connect;
class job_thread:public base_net_thread
{
    public:
        job_thread(){
            _job_thread_vec.push_back(this);
        };

        virtual ~job_thread(){
        };

        virtual bool handle_msg(base_passing_msg * msg);
        
        static void put_msg(base_passing_msg * msg);

    protected:
        static vector<job_thread *> _job_thread_vec;

};

#endif
