#ifndef __NET_OBJ_CONTAINER_H__
#define __NET_OBJ_CONTAINER_H__
#include "common_epoll.h"
#include <map>
#include "base_net_container.h"
using namespace std;
const int DEFAULT_CHECK_INTERVAL = 180*1000;

class inner_obj_container:public base_net_container
{
	public:
		inner_obj_container()
		{
			_p_epoll = NULL;
		}
		
		virtual ~inner_obj_container()
		{
			if (_p_epoll != NULL)
				delete _p_epoll;
		}
	
		
		void insert(base_net_obj *p_obj);
		
		void register_with_thread(pthread_t tid, base_net_obj * p_net_obj);
		
		base_net_obj* find(uint64_t obj_id);
		
		bool erase(uint64_t obj_id);

		virtual void obj_process();

		void push_net_obj(base_net_obj *p_obj);
	protected:
		map<pthread_t, base_net_obj> _thd_channel;
		map<uint64_t, base_net_obj*> _obj_map;
		map<uint64_t, base_net_obj*> _obj_net_map;
		
		//common_epoll* _p_epoll;
};


#endif

