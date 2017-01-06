#ifndef __COMMON_OBJ_CONTAINER_H__
#define __COMMON_OBJ_CONTAINER_H__

#include "base_def.h"
#include "common_epoll.h"
#include "base_net_container.h"


class common_obj_container:public base_net_container
{
	public:
		common_obj_container()
		{
		}
		
		virtual ~common_obj_container()
		{
		}
		
		virtual base_net_obj* find(obj_id_str * obj_id);
		
		virtual bool erase(obj_id_str *obj_id);

		virtual void obj_process();

		void push_net_obj(base_net_obj *p_obj);
	protected:
		map<obj_id_str, base_net_obj*> _obj_net_map;
};


#endif

