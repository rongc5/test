#ifndef __LISTEN_CONNECT_H__
#define __LISTEN_CONNECT_H__

#include "common_epoll.h"
#include <string>
#include "base_obj_id.h"
#include "base_connect.h"
using namespace std;

template<class LISTEN_PROCESS>
class listen_connect:public base_net_obj
{
	public:
		listen_connect()
		{
			_port = 0;
			_process = NULL;
		}

		~listen_connect()
		{	
		}


		void init(const string &ip, unsigned short port)
		{
			_ip = ip;
			_port = port;
			struct sockaddr_in address;
			int reuse_addr = 1;

			memset((char *) &address, 0, sizeof(address));
			address.sin_family = AF_INET;
			address.sin_port = htons(port);
			int ret = 0;

			if (ip != "")
			{
				inet_aton(ip.c_str(), &address.sin_addr);
			}
			else
			{
				address.sin_addr.s_addr = htonl(INADDR_ANY);
			}

			_sock = socket(AF_INET, SOCK_STREAM, 0);
			if (_sock < 0) 
			{
				THROW_COMMON_EXCEPT(errno, "socket error " << strerror(errno));     
			}
			setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, (void*)(&(reuse_addr)), sizeof(reuse_addr));

			if (::bind(_sock, (struct sockaddr *) &address, sizeof(address)) < 0) 
			{	     
				THROW_COMMON_EXCEPT(errno, "bind error "  << strerror(errno) << " " << ip << ":" << port);
			}        

			ret = listen(_sock, 250);
			if (ret == -1)
			{
				THROW_COMMON_EXCEPT(errno, "listen error "  << strerror(errno));
			}
			_epoll_event = EPOLLIN | EPOLLERR | EPOLLHUP;
		}

		int real_net_process()
		{
			return 0;
		}

		void event_process(int events)
		{	
			if ((events & EPOLLIN) == EPOLLIN)
			{
				int tmp_sock = 0;
				sockaddr_in addr;	
				socklen_t len = 0;
				while((tmp_sock = accept(_sock, (sockaddr*)&addr, &len)) != -1)
				{				
					if (addr.sin_addr.s_addr == 0)
					{
						getpeername(tmp_sock, (sockaddr*)&addr, &len);
					}
					_process->process(tmp_sock, addr);
				}
			}
			else
			{
			}
		}		

		void  handle_timeout(const uint32_t timer_type)
		{
			return;
		}


		int destroy() 
		{
			delete this;
			return 0;
		}

		void process(normal_obj_msg *p_msg)
		{
			return;
		}

		void set_process(LISTEN_PROCESS *p)
		{
			if (_process != NULL)
				delete _process;
			_process = p;
		}

		void set_thread_index(const uint16_t thread_index)
		{
		}

		uint16_t get_thread_index() 
		{
			return 0;
		}
	private:
		string _ip;
		unsigned short _port;
		LISTEN_PROCESS *_process;
};
#endif

