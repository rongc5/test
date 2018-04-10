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

        virtual bool push_real_net(base_net_obj *p_obj);
        virtual bool remove_real_net(base_net_obj *p_obj);

        virtual base_net_obj* find(const ObjId * obj_id);

        virtual bool insert(base_net_obj *p_obj);
        virtual bool erase(const ObjId *obj_id);

        virtual void put_msg(ObjId & id, std::shared_ptr<normal_msg> & p_msg);

        virtual void obj_process();

    protected:
        std::unordered_map<uint64_t, base_net_obj*> _obj_map;
        std::unordered_map<uint64_t, base_net_obj*> _obj_net_map;
};

#endif

