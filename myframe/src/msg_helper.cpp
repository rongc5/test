#ifndef __MSG_HELPER_H__
#define __MSG_HELPER_H__



namespace MZFRAME {
	
struct obj_id_str;



class base_net_msg {
	
	public:
		base_net_msg();
		virtual ~base_net_msg();
		virtual void event_process(const int32_t events) = 0;
		
	
	protected:
		int _sfd;
		uint64_t _obj_id;
		int _epoll_event;
		common_epoll *_epoll;
};


class add_net_msg {
		public:
			add_net_msg();
			virtual ~add_net_msg();
			virtual void event_process(const int32_t events);
}


typedef struct _msg_t
{		 
	   uint64_t _obj_id; //消息id
     uint64_t _obj_op; //消息命令
     unsigned short version; //
     unsigned int reserved; //保留字段
     unsigned int body_len;
} msg_t;
	

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
	
	
	
	