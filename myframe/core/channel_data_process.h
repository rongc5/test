#ifndef _CHANNEL_DATA_PROCESS_H_
#define _CHANNEL_DATA_PROCESS_H_

#include "common_def.h"
#include "thread_helper.h"

class base_connect;
class base_data_process;
class channel_data_process:public base_data_process
{
    public:
        channel_data_process(base_connect *p);

        virtual ~channel_data_process()
        {
            deque<normal_obj_msg >::iterator it;
            for (it = _queue.begin(); it != _queue.end(); it++) {
                delete it->p_msg;
            }
        }	

        virtual size_t process_recv_buf(char *buf, size_t len);

        virtual bool process_recv_msg(ObjId & id, normal_msg * p_msg);
    

    protected:
        thread_mutex_t _mutex;
        deque<normal_obj_msg > _queue;
};

#endif

