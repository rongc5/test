#ifndef __COMMON_OBJ_CONTAINER_H__
#define __COMMON_OBJ_CONTAINER_H__

#include "common_def.h"
#include "common_epoll.h"
#include "base_net_container.h"
#include "inc_dict.h"

class base_net_thread;
class common_obj_container:public base_net_container
{
    public:
        common_obj_container(base_net_thread * thread, uint32_t obj_num);

        void init(uint32_t obj_num);

        virtual ~common_obj_container();

        virtual bool push_real_net(base_net_obj *p_obj);
        virtual bool remove_real_net(base_net_obj *p_obj);

        virtual base_net_obj* find(const ObjId * obj_id);

        virtual bool insert(base_net_obj *p_obj);
        virtual bool erase(const ObjId *obj_id);

        virtual void put_msg(ObjId & id, normal_msg * p_msg);

        virtual void obj_process();

    protected:
        inc_dict_t<base_net_obj*> *_obj_map;
        inc_dict_t<base_net_obj*> *_obj_net_map;
};

#endif

