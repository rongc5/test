#ifndef __BASE_NET_THREAD_H__
#define __BASE_NET_THREAD_H__

#include "common_def.h"
#include "base_thread.h"

class base_connect;
class base_net_thread:public base_thread
{
    public:
        base_net_thread():_channelid(0){
            _base = event_base_new();
        };

        virtual ~base_net_thread(){
            close(_channelid);
        };

        virtual void *run();

        virtual void init();

        virtual void put_msg(base_passing_msg * p_msg);

        virtual void routine_msg();

        static void channel_cb(int fd, short ev, void *arg);

        /************* *****************/

        virtual void handle_new_msg(base_passing_msg * p_msg) = 0;

        struct event_base * get_event_base()
        {
            return _base;
        }

        void add_connect_map(base_connect * _conn);

        void destory_connect(ObjId  id);

        base_connect * get_connect(ObjId   id);

        const ObjId & gen_id_str();


        static void passing_msg(base_passing_msg * msg);

        static base_net_thread * get_base_net_thread_obj(uint32_t thread_index);

    protected:
        struct event_base* _base;
        int _channelid;
        ObjId _id_str;
        deque<base_passing_msg *> _queue;
        thread_mutex_t _base_net_mutex;
        map<ObjId, base_connect*> _connect_map;

        typedef typename map<uint32_t, base_net_thread *>::iterator bntMapIter;
        static map<uint32_t, base_net_thread *> _base_net_thread_map;
};

#endif
