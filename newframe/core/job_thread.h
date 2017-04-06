#ifndef __JOB_THREAD_H__
#define __JOB_THREAD_H__

#include "common_def.h"
#include "base_net_thread.h"

class base_connect;
class job_thread:public base_net_thread
{
    public:
        job_thread():_channelid(0){
        };

        virtual ~job_thread(){
        };

        virtual void handle_new_msg(base_passing_msg * p_msg);
};

#endif
