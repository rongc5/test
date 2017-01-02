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
		
		base_net_obj* find(uint64_t obj_id);
		
		bool erase(uint64_t obj_id);

		virtual void obj_process();

		void push_net_obj(base_net_obj *p_obj);
	protected:
		map<uint64_t, base_net_obj*> _obj_net_map;
};


#endif

