#ifndef _CHANNEL_DATA_PROCESS_H_
#define _CHANNEL_DATA_PROCESS_H_

#include "common_def.h"
#include "base_data_process.h"
#include "thread_helper.h"


class channel_data_process:public base_data_process
{
    public:
        channel_data_process(void *p);

        virtual ~channel_data_process()
        {
            deque<normal_obj_msg *>::iterator it;
            for (it = _queue.begin(); it != _queue.end(); it++) {
                delete *it;
            }
        }	

        virtual size_t process_recv_buf(char *buf, size_t len);

        virtual bool process_recv_msg(ObjId & id, normal_msg * p_msg);
    

    protected:
        thread_mutex_t _mutex;
        deque<normal_obj_msg *> _queue;
};

#endif

