#ifndef __COMMON_THREAD_H__
#define __COMMON_THREAD_H__

#include "def.h"

using namespace std;

class epoll_helper;


struct obj_id_str {
	obj_id_str()
	{
		_obj_id = 0;
		_obj_op = 0;
		_thread_id = 0;
	}
	
	uint64_t _obj_id; //消息id
	uint64_t _obj_op; //消息命令
	pthread_t _thread_id;//线程id
	  
	
	
	bool operator==(const obj_id_str &o1) const
	{
		return _thread_id == o1._thread_id && _obj_id == o1._obj_id && _obj_op == o1._obj_op;
	}
	
	
	bool operator<(const obj_id_str &o1) const
	{
		if (_obj_id != o1._obj_id){
			return 	_obj_id < o1._obj_id;
		}
		else if (_obj_op < o1._obj_op)
		{
			return _obj_op_obj_op < o1._obj_op;
		}
		else 
		{
			return _thread_id < o1._thread_id;
		}
	}
		
	};


class common_thread:public base_thread
{
	public:
		common_thread();
		~common_thread();
		void init();
		int put_msg(pthread_t thd, base_obj *p_obj);
		void *run();
	protected:
		epoll_helper * _epoll;
		static map<pthread_t, int> _thd_fd_map;
		int _thd_fd;
		uint64_t _obj_id;
};


#endif
