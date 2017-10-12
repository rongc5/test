#ifndef __BASE_NET_CONTAINER_H_
#define __BASE_NET_CONTAINER_H_

#include "common_def.h"


class base_net_obj;
class common_epoll;
class base_timer;
class base_net_thread;
class base_net_container
{
    public:
        base_net_container(base_net_thread * thread);
        virtual ~base_net_container();

        virtual bool push_real_net(base_net_obj *p_obj) = 0;
        virtual bool remove_real_net(base_net_obj *p_obj) = 0;
        virtual base_net_obj* find(const ObjId * obj_id) = 0;

        virtual bool insert(base_net_obj *p_obj) = 0;
        virtual bool erase(const ObjId *_obj_id) = 0;
        virtual void obj_process() = 0;
        virtual void put_msg(ObjId & id, normal_msg * p_msg) = 0;

        common_epoll *get_epoll();
        base_net_thread * get_net_thread();

        base_timer * get_timer();

        void init(const uint32_t epoll_size = 0);

    protected:
        const ObjId & gen_id_str();

    protected:

        base_net_thread * _net_thread;
        common_epoll *_p_epoll;
        base_timer * _timer;
        ObjId _id_str;
};


#endif

