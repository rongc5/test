#ifndef _CHANNEL_DATA_PROCESS_H_
#define _CHANNEL_DATA_PROCESS_H_

#include "common_def.h"
#include "base_data_process.h"

class base_net_obj;
class channel_data_process:public base_data_process
{
    public:
        channel_data_process(base_net_obj *p);

        virtual ~channel_data_process()
        {
            std::deque<normal_obj_msg >::iterator it;
            for (it = _queue.begin(); it != _queue.end(); it++) {
                delete it->p_msg;
            }
        }	

        virtual size_t process_recv_buf(const char *buf, size_t len);

        virtual bool process_recv_msg(ObjId & id, normal_msg * p_msg);
    

    protected:
        std::mutex _mutex;
        std::deque<normal_obj_msg > _queue;
};

#endif

