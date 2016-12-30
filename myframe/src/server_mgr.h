#ifndef __SERVER_MGR_H__
#define __SERVER_MGR_H__
#include "obj_mgr.h"
#include <string>
#include "net_obj_container.h"
#include "base_connect.h"
#include "obj_thread.h"
#include "listen_base.h"
#include "ssl_base_connect.h"
using namespace std;

struct server_extra_info
{
     bool _is_ssl;
     speed_limit_value  _limit_speed;
     uint8_t _epoll_type;
     uint32_t _epoll_size;
     server_extra_info(const uint32_t tmp = 0)
     {
        _is_ssl = (tmp == 0)?false:true;
        _limit_speed = tmp;
        _epoll_type = tmp;
        _epoll_size = tmp;
     }
};

template<class PROCESS>
class base_connect_in:public base_connect<PROCESS>
{
	public:
		base_connect_in(const int32_t sock, const connect_info &info):base_connect<PROCESS>(sock, info)
		{
		}
		
		connect_info* gen_connect_info()
		{
			return NULL;
		}
	private:	
};

template<class PROCESS>
class ssl_base_connect_in:public ssl_base_connect<PROCESS>
{
	public:
		ssl_base_connect_in(const int32_t sock, const connect_info &info)
			:ssl_base_connect<PROCESS>(sock, info)
		{
		}
		
		connect_info* gen_connect_info()
		{
			return NULL;
		}
	private:	
};


template<class SERVER_MGR>
struct listen_process
{
public:
	listen_process(SERVER_MGR *p)
	{
		_p_mgr = p;
	}
		
	int process(const int sock, const sockaddr_in &addr)
	{
		_p_mgr->add_sock(sock, addr);
		return 0;
	}
private:
	SERVER_MGR *_p_mgr;
};

template<class MGR_PROCESS,  class GEN_PASSIVE_CONNECT_PROCESS>
class server_mgr;

template<class MSG_PROCESS>
class default_gen_passive_connect_process
{
    public:
           default_gen_passive_connect_process(server_mgr<MSG_PROCESS, default_gen_passive_connect_process<MSG_PROCESS> > *p_mgr)
		   {
				_p_mgr = p_mgr;
		   }

           NET_OBJ *gen_connect(const int sock, const sockaddr_in &addr)
		   {
			   return _p_mgr->gen_connect(sock, addr);
		   }

           static default_gen_passive_connect_process<MSG_PROCESS>* gen_process(
                server_mgr<MSG_PROCESS, default_gen_passive_connect_process<MSG_PROCESS> > *p_mgr)
		   {
			   default_gen_passive_connect_process<MSG_PROCESS> *p_gen = new default_gen_passive_connect_process<MSG_PROCESS>(p_mgr);
			   return p_gen;
		   }

    private:
           server_mgr<MSG_PROCESS, default_gen_passive_connect_process<MSG_PROCESS> > *_p_mgr;
};

template<class MSG_PROCESS>
class long_gen_passive_connect_process
{
public:
	long_gen_passive_connect_process(server_mgr<MSG_PROCESS, long_gen_passive_connect_process<MSG_PROCESS> > *p_mgr)
	{
		_p_mgr = p_mgr;
	}

	NET_OBJ *gen_connect(const int sock, const sockaddr_in &addr)
	{
		//设置keepalive时间短点
		int ikeepAlive = 1; // 开启keepalive属性
		int ikeepIdle = 10; // 如该连接在10秒内没有任何数据往来,则进行探测
		int ikeepInterval = 5; // 探测时发包的时间间隔为5 秒
		int ikeepCount = 3; // 探测尝试的次数.如果第1次探测包就收到响应了,则后2次的不再发.

		setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, ( void * )&ikeepAlive, sizeof( ikeepAlive ) );
		setsockopt(sock, SOL_TCP, TCP_KEEPIDLE, ( void* )&ikeepIdle, sizeof( ikeepIdle ) );
		setsockopt(sock, SOL_TCP, TCP_KEEPINTVL, ( void * )&ikeepInterval, sizeof( ikeepInterval ) );
		setsockopt(sock, SOL_TCP, TCP_KEEPCNT, ( void * )&ikeepCount, sizeof( ikeepCount ) );

		return _p_mgr->gen_connect(sock, addr);
	}

	static long_gen_passive_connect_process<MSG_PROCESS>* gen_process(
		server_mgr<MSG_PROCESS, long_gen_passive_connect_process<MSG_PROCESS> > *p_mgr)
	{
		long_gen_passive_connect_process<MSG_PROCESS> *p_gen = new long_gen_passive_connect_process<MSG_PROCESS>(p_mgr);
		return p_gen;
	}

private:
	server_mgr<MSG_PROCESS, long_gen_passive_connect_process<MSG_PROCESS> > *_p_mgr;
};

#define LISTEN_PROCESS_IN listen_process<server_mgr<MSG_PROCESS, GEN_PASSIVE_CONNECT_PROCESS> >

template<class MSG_PROCESS, class GEN_PASSIVE_CONNECT_PROCESS=default_gen_passive_connect_process<MSG_PROCESS> >
class server_mgr
{
	public:
		server_mgr():_thread_num(0), _server_ip(""), _server_port(0), _extra_info(0)
		{
		}

		server_mgr(const int num, const string &ip, const unsigned short port,  const server_extra_info &info = 0)
		{
                    init(num, ip, port,  info);
		}
		
		virtual ~server_mgr()
		{
		}

		void init(const int num, const string &ip, const unsigned short port, const server_extra_info &info)
		{
			_thread_num = num;
			_server_ip = ip;
			_server_port = port;
			_extra_info = info;
                     _p_gen_process = NULL;
		}
		
		void run()
		{
		      _p_gen_process = GEN_PASSIVE_CONNECT_PROCESS::gen_process(this);
                    
			//线程对象
			_p_obj_thread_arr = new obj_thread<net_obj_container>[_thread_num];			
			//处理对象线程
			for(int ii=0; ii < _thread_num; ii++)
			{
				_p_obj_thread_arr[ii].start(_extra_info._epoll_size);
			}

                     //监控线程
			listen_base<LISTEN_PROCESS_IN > *p_listen = new listen_base<LISTEN_PROCESS_IN >();
			LISTEN_PROCESS_IN *process = new LISTEN_PROCESS_IN(this);
			p_listen->set_process(process);			
			p_listen->init(_server_ip, _server_port);			
			_listen_obj_thread.start();
			add_obj_msg *p_msg = new add_obj_msg();
			p_msg->_p_obj = p_listen;					
			_listen_obj_thread.put_msg(p_msg);
		}
		
		void put_msg(normal_obj_msg *p_msg)
		{		
			int index = p_msg->_obj_id %_thread_num;			
			_p_obj_thread_arr[index].put_msg(p_msg);
		}

		obj_id_str put_obj(NET_OBJ *p)
		{
			add_obj_msg *p_msg = new add_obj_msg();
			p_msg->_p_obj = p;
			uint32_t index = p->get_id() % _thread_num;
			obj_id_str ret_str = p->get_obj_str();
			_p_obj_thread_arr[index].put_msg(p_msg);
			return ret_str;
		}

		void add_sock(const int32_t sock,  const sockaddr_in &addr) 
		{
			NET_OBJ *p_connect = _p_gen_process->gen_connect(sock, addr);
			if (p_connect != NULL)
				put_obj(p_connect);       		
			return;
		}

		/*
		  const server_extra_info &get_extra_info()
		  {
			  return _extra_info;
		  }
		  */


		  NET_OBJ * gen_connect(const int sock, const sockaddr_in &addr)
		  {
			  connect_info info;
			  info._addr = addr;
			  info._speed = _extra_info._limit_speed;
			  info._epoll_type = _extra_info._epoll_type;
			  NET_OBJ *p_connect = NULL;
			  if (_extra_info._is_ssl)//ssl
			  {
				  p_connect = new ssl_base_connect_in<MSG_PROCESS>(sock, info);
				  MSG_PROCESS *process = new MSG_PROCESS(p_connect);
				  ssl_base_connect_in<MSG_PROCESS>* tmp_con = ( ssl_base_connect_in<MSG_PROCESS>*)p_connect;
				  tmp_con->set_process(process);

			  }
			  else
			  {
				  p_connect = new base_connect_in<MSG_PROCESS>(sock, info);
				  MSG_PROCESS *process = new MSG_PROCESS((NET_OBJ*)p_connect);
				  base_connect_in<MSG_PROCESS>* tmp_con = (base_connect_in<MSG_PROCESS>*)p_connect;
				  tmp_con->set_process(process);       			
			  }
			  return p_connect;
		  }

	private:
		obj_thread<net_obj_container> _listen_obj_thread;
		obj_thread<net_obj_container> *_p_obj_thread_arr;
		int _thread_num;
		string _server_ip;
		unsigned short _server_port;
		server_extra_info _extra_info;
              GEN_PASSIVE_CONNECT_PROCESS *_p_gen_process;
};

#endif

