#ifndef _CHANNEL_DATA_PROCESS_H_
#define _CHANNEL_DATA_PROCESS_H_

#include "common_def.h"
#include "base_data_process.h"

class normal_obj_msg //内部传递的消息
{
    public:
        uint32_t _id;
        std::shared_ptr<normal_msg>  p_msg;

        virtual ~normal_obj_msg(){
        }   
};


class base_net_obj;
class channel_data_process:public base_data_process
{
    public:
        channel_data_process(std::shared_ptr<base_net_obj> p, int channelid);

        virtual ~channel_data_process()
        {
            _queue.clear();
        }	

        virtual size_t process_recv_buf(const char *buf, size_t len);

        void put_msg(uint32_t obj_id, std::shared_ptr<normal_msg> & p_msg);

    protected:
        std::mutex _mutex;
        std::deque<normal_obj_msg > _queue;
        int _channelid;
};

#endif

