#ifndef __LISTEN_THREAD_H__
#define __LISTEN_THREAD_H__

#include "base_net_thread.h"
#include "common_def.h"

class listen_thread: public base_net_thread
{
    public:
        listen_thread():base_net_thread(0),_current_indx(0){
        };
        virtual ~listen_thread(){
        };

        void init(const string &ip, unsigned short port);

        int add_worker_thread(base_net_thread * thread);

        virtual void add_msg(base_passing_msg * p_msg);

        virtual bool handle_msg(base_passing_msg * msg);

    protected:

        string _ip;
        unsigned short _port;

        vector<base_net_thread * > _worker_thrds;
        uint32_t _current_indx;
};

#endif
