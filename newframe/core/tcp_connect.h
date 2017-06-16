#ifndef __TCP_CONNECT_H__
#define __TCP_CONNECT_H__

#include "log_helper.h"
#include "base_connect.h"
#include "base_net_thread.h"
#include "common_def.h"

class base_net_thread;
class tcp_connect:public base_connect
{
    public:

        tcp_connect(int32_t sock, base_net_thread * thread):base_connect(sock, thread), _send_buf(NULL)
        {
        }

        virtual ~tcp_connect();

        virtual size_t process_recv_buf(char *buf, size_t len) = 0;

        virtual void notice_send();

    protected:

        virtual void call_back(int fd, short ev, void *arg);

        void real_recv();

        virtual int RECV(void *buf, size_t len);

        void real_send();

        virtual ssize_t SEND(const void *buf, const size_t len);

    protected:
        string _recv_buf;

        string * _send_buf;
};


#endif
