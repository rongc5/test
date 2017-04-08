#ifndef __CLIENT_CONNECT_H__
#define __CLIENT_CONNECT_H__

#include "log_helper.h"
#include "common_connect.h"
#include "base_net_thread.h"

class base_net_thread;
class client_connect: public common_connect
{
    public:

        client_connect(int32_t sock, base_net_thread * thread):common_connect(sock, thread)
        {

        }

        virtual ~client_connect()
        {
        }

        static client_connect * gen_connect(const string &ip, unsigned short port, base_net_thread * thread);

        virtual void call_back(int fd, short ev, void *arg);

        void send_request();
};


#endif
