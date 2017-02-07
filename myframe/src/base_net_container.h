#ifndef __BASE_NET_CONTAINER_H_
#define __BASE_NET_CONTAINER_H_

#include "common_def.h"


class base_net_obj;
class common_epoll;

class base_net_container
{
    public:
        base_net_container();
        virtual ~base_net_container();

        virtual void push_net_obj(base_net_obj *p_obj) = 0;
        virtual base_net_obj* find(const ObjId * obj_id) = 0;
        virtual bool erase(ObjId *_obj_id) = 0;
        virtual void obj_process() = 0;

        common_epoll *get_epoll();

        void init(const uint32_t epoll_size = 0);

    protected:

        common_epoll *_p_epoll;
};


#endif

