#ifndef __CHANNEL_CONNECT_H__
#define __CHANNEL_CONNECT_H__

#include "log_helper.h"
#include "base_net_thread.h"
#include "common_connect.h"
#include "common_def.h"


class base_net_thread;
class channel_connect:public common_connect
{
    public:

        channel_connect(int32_t sock, base_net_thread * thread):common_connect(sock, thread)
        {

        }

        virtual ~channel_connect()
        {
        }
        

        virtual void call_back(int fd, short ev, void *arg);

        static channel_connect * gen_connect(int fd, base_net_thread * thread);
};


#endif