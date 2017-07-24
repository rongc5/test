#ifndef __COMMON_OBJ_CONTAINER_H__
#define __COMMON_OBJ_CONTAINER_H__

#include "common_def.h"
#include "common_epoll.h"
#include "base_net_container.h"

class base_net_thread;
class common_obj_container:public base_net_container
{
    public:
        common_obj_container(base_net_thread * thread);

        virtual ~common_obj_container();

        virtual void push_net_obj(base_net_obj *p_obj);

        virtual void remove_net_obj(base_net_obj *p_obj);

        virtual base_net_obj* find(const ObjId * obj_id);

        virtual bool erase(ObjId *obj_id);

        virtual void put_msg(ObjId & id, normal_msg * p_msg);

        virtual void obj_process();


    protected:
        map<ObjId, base_net_obj*> _obj_net_map;
};

#endif

