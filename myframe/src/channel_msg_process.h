#ifndef _CHANNEL_MSG_PROCESS_H_
#define _CHANNEL_MSG_PROCESS_H_

#include "base_def.h"
#include "common_def.h"
#include "common_thread.h"
#include "base_msg_process.h"


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

    void set_common_thread(common_thread *thread);

    common_thread* get_common_thread();

    protected:
    common_thread *_thread;
};


#endif

