#ifndef __BASE_CONNECT_H__
#define __BASE_CONNECT_H__

#include "log_helper.h"
#include "base_net_thread.h"
#include "common_def.h"
#include "common_exception.h"

class base_net_thread;
class base_connect
{
    public:

        base_connect(int32_t sock, base_net_thread * thread):_fd(sock), _thread(thread)
        {

        }

        virtual ~base_connect()
        {
            close();
        }

        void init_ev(short ev);


        void close();

        int get_sock();
        
        int destroy();

        void get_local_addr(sockaddr_in &addr);

        virtual void call_back(int fd, short ev, void *arg) = 0;

        static void on_cb(int fd, short ev, void *arg);

    protected:
        int _fd;
        base_net_thread * _thread;
        struct event _event;
};


#endif
