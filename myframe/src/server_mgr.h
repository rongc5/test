#ifndef __SERVER_MGR_H__
#define __SERVER_MGR_H__

#include "base_connect.h"
#include "listen_thread.h"
#include "listen_base.h"


template<class MSG_PROCESS>
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
		}
		
		void run()
		{

            _listen_obj_thread = new listen_thread();
            _listen_obj_thread->init(_server_ip, _server_port);
                    
            passing_msg_thread::register_thread(_listen_obj_thread);




            obj_thread<net_obj_container> _listen_obj_thread;
            //线程对象
            _p_obj_thread_arr = new obj_thread<net_obj_container>[_thread_num];			
            //处理对象线程
            for(int ii=0; ii < _thread_num; ii++)
            {
                _p_obj_thread_arr[ii].start(_extra_info._epoll_size);
            }

		}

	private:
        listen_thread * _listen_obj_thread;
		obj_thread<net_obj_container> *_p_obj_thread_arr;
		int _thread_num;
		string _server_ip;
		unsigned short _server_port;
};

#endif

