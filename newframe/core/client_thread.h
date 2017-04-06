#ifndef __LISTEN_THREAD_H__
#define __LISTEN_THREAD_H__

#include "job_thread.h"
#include "common_def.h"

class listen_thread: public job_thread
{
    public:
        listen_thread():_current_indx(0){
        };
        virtual ~listen_thread(){
        };

        void init(const string &ip, unsigned short port);

        int add_worker_thread(base_net_thread * thread);

        virtual void handle_new_msg(base_passing_msg * p_msg);

    protected:

        string _ip;
        unsigned short _port;

        vector<base_net_thread * > _worker_thrds;
        uint32_t _current_indx;
};

#endif
