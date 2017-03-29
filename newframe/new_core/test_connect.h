#ifndef __TEST_CONNECT_H__
#define __TEST_CONNECT_H__

#include "log_helper.h"
#include "base_connect.h"
#include "base_net_thread.h"


class base_net_thread;
class test_connect:public base_connect
{
    public:

        test_connect(int32_t sock, base_net_thread * thread):_fd(sock), _thread(thread)
        {

        }

        virtual ~test_connect()
        {
        }
        

        virtual void call_back(int fd, short ev, void *arg);

        void real_recv();

        int RECV(void *buf, size_t len);

        static test_connect * gen_connect(int fd, base_net_thread * thread);


        size_t process_recv_buf(char *buf, size_t len);

    protected:
        string _recv_buf;
        size_t _recv_buf_len;
};


#endif
