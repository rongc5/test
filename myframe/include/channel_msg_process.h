#ifndef _CHANNEL_MSG_PROCESS_H_
#define _CHANNEL_MSG_PROCESS_H_

#include "base_def.h"
#include "common_def.h"
#include "common_net_thread.h"
#include "base_msg_process.h"


class base_net_thread;
class channel_msg_process:public base_msg_process
{
    public:
    channel_msg_process(void *p):base_msg_process(p), _thread(NULL)
    {
    }

    virtual ~channel_msg_process()
    {
    }	

    virtual size_t process_recv_buf(char *buf, size_t len);

    size_t process_s(char *buf, size_t len);

    void put_msg(char * buf, size_t len);

    void set_base_net_thread(base_net_thread *thread);

    base_net_thread* get_base_net_thread();

    protected:
        base_net_thread *_thread;
};


#endif

