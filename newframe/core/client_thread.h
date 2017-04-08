#ifndef __CLIENT_THREAD_H__
#define __CLIENT_THREAD_H__

#include "base_net_thread.h"
#include "common_def.h"

class client_thread: public base_net_thread
{
    public:
        client_thread(){
        };
        virtual ~client_thread(){
        };

        void init(const string &ip, unsigned short port);

        virtual void handle_new_msg(base_passing_msg * p_msg);

    protected:

        string _ip;
        unsigned short _port;
};

#endif
