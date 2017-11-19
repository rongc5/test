#ifndef __BASE_NET_THREAD_H__
#define __BASE_NET_THREAD_H__

#include "common_def.h"
#include "base_thread.h"
#include "common_obj_container.h"
#include "event_channel_msg.h"

class base_net_container;
class base_net_thread:public base_thread
{
    public:
        base_net_thread(int channel_num = 1, uint32_t obj_num=5003);
        virtual ~base_net_thread();

        virtual void *run();

        virtual void net_thread_init();

        virtual void put_msg(ObjId & id, normal_msg * p_msg);

        static void put_obj_msg(ObjId & id, normal_msg * p_msg);

        virtual void handle_msg(normal_msg * p_msg);

        static base_net_thread * get_base_net_thread_obj(uint32_t thread_index);

    protected:

        int _channel_num;
        vector<event_channel_msg *> _channel_msg_vec;

        base_net_container * _base_container;

        typedef typename map<uint32_t, base_net_thread *>::iterator bntMapIter;
        static map<uint32_t, base_net_thread *> _base_net_thread_map;
};

#endif
