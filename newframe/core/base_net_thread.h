#ifndef __BASE_NET_THREAD_H__
#define __BASE_NET_THREAD_H__

#include "common_def.h"
#include "base_thread.h"

class event_channel_msg;
class base_connect;
class base_net_thread:public base_thread
{
    public:
        base_net_thread(int channel_num = 1):_channel_num(channel_num){
            _base = event_base_new();
        };

        virtual ~base_net_thread(){
        };

        virtual void *run();

        virtual void add_msg(base_passing_msg * p_msg);

        struct event_base * get_event_base()
        {
            return _base;
        }

        void add_connect_map(base_connect * _conn);

        void destory_connect(ObjId  id);

        base_connect * get_connect(ObjId   id);

        const ObjId & gen_id_str();

        static base_net_thread * get_base_net_thread_obj(uint32_t thread_index);

        virtual bool handle_msg(base_passing_msg * msg) = 0;

    protected:

        void init();

        static void on_cb(int fd, short ev, void *arg);

    protected:
        struct event_base* _base;
        ObjId _id_str;
        map<ObjId, base_connect*> _connect_map;

        int _channel_num;
        vector<event_channel_msg *> _channel_msg_vec;

        typedef typename map<uint32_t, base_net_thread *>::iterator bntMapIter;
        static map<uint32_t, base_net_thread *> _base_net_thread_map;
};

#endif
