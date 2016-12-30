#ifndef __NET_OBJ_H__
#define __NET_OBJ_H__

#define "base_def.h"


class common_epoll;

class connect_info
{
	public:
		connect_info()
		{
			_epoll_type = 0;
		}
		virtual ~connect_info()
		{
		}
		
	public:	
		sockaddr_in _addr;
		uint8_t _epoll_type;
};

struct obj_id_str;

class base_net_obj
{
public:
	base_net_obj()
	{
		_fd = 0;
		_epoll_event = 0;
		_p_net_container = NULL;
		_p_epoll = NULL;
	}
	
	virtual ~base_net_obj()
	{
	}
	
	virtual void event_process(const int32_t events) = 0;
	virtual int destroy() = 0;
	
	virtual void process()=0;
	
	virtual void set_net_container(base_net_container *p_net_container);
	
	virtual void put_msg(string *p_msg) = 0
	
	size_t process_recv_buf(char *buf, size_t len) = 0;

	virtual int get_event()
	{
		return _epoll_event
	}
	
	int get_sock()
	{
		return _fd;
	}

    int set_id_str(obj_id_str & id_str)
    {
        _id_str = id_str;
        return 0;
    }
	
	const obj_id_str & get_id_str()
    {
		return _id_str;
	}
	
protected:
	base_net_container *_p_net_container;
	common_epoll *_p_epoll;
	int _epoll_event;
	int _fd;	
	obj_id_str _id_str;
};

class NET_OBJ:public base_net_obj
{
public:
	NET_OBJ()
	{
		memset(&_peer_addr, 0, sizeof(_peer_addr));
		_p_epoll = NULL;
		_epoll_event = 0;
	}
	virtual ~NET_OBJ()
	{		
	}
	virtual void close()=0;	
	virtual void get_local_addr(sockaddr_in &addr)=0;


	/*******************************************************/	
	void get_peer_addr(sockaddr_in &addr);
	
protected:
	sockaddr_in _peer_addr;	
};


#endif

