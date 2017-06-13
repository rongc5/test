#ifndef __JOB_CONNECT_H__
#define __JOB_CONNECT_H__

#include "log_helper.h"
#include "tcp_connect.h"
#include "base_net_thread.h"
#include "common_def.h"

class base_net_thread;
class job_connect:public tcp_connect
{
    public:

        job_connect(int32_t sock, base_net_thread * thread):tcp_connect(sock, thread)
        {

        }

        virtual ~job_connect();

        static job_connect * gen_connect(int fd, base_net_thread * thread);

        virtual void recv_passing_msg(base_passing_msg * p_msg);

        size_t process_recv_buf(char *buf, size_t len);

        size_t process_s(char *buf, size_t len);
};


#endif
