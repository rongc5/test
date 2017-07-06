#ifndef __BASE_NET_THREAD_H__
#define __BASE_NET_THREAD_H__

#include "common_def.h"
#include "base_thread.h"
#include "common_obj_container.h"
#include "net_obj.h"
#include "base_connect.h"
#include "event_channel_msg.h"

class base_net_thread:public base_thread
{
    public:
        base_net_thread(int channel_num = 1)::_channel_num(channel_num), _base_container(NULL){
            _base_container = new common_obj_container(this);
        };
        virtual ~base_net_thread(){
            if (_base_container){
                delete _base_container;
            }
        };

        virtual void *run();

        virtual void init();

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
