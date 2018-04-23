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
        base_net_thread(int channel_num = 1);
        virtual ~base_net_thread();

        virtual void *run();

        virtual void run_process();

        virtual void net_thread_init();

        virtual void put_msg(ObjId & id, std::shared_ptr<normal_msg> & p_msg);

        static void put_obj_msg(ObjId & id, std::shared_ptr<normal_msg> & p_msg);

        virtual void handle_msg(std::shared_ptr<normal_msg> & p_msg);

        static base_net_thread * get_base_net_thread_obj(uint32_t thread_index);

        void add_timer(timer_msg & t_msg);

        //true /false
        virtual bool handle_timeout(timer_msg & t_msg);

    protected:

        int _channel_num;
        std::vector<event_channel_msg *> _channel_msg_vec;

        common_obj_container * _base_container;

        static std::unordered_map<uint32_t, base_net_thread *> _base_net_thread_map;
};

#endif
