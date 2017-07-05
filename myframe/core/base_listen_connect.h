#ifndef __BASE_LISTEN_CONNECT_H__
#define __BASE_LISTEN_CONNECT_H__

#include "common_def.h"


class base_net_obj;
class base_listen_process;
class base_listen_connect:public base_net_obj
{
	public:
		base_listen_connect();

		virtual ~base_listen_connect();
		{	
		}

		virtual void event_process(int events);

		void  handle_timeout(const uint32_t timer_type);

        void set_process(base_listen_process *p);

         virtual size_t process_recv_msg(normal_obj_msg * p_msg);


	private:
        base_listen_process *_process;
};


#endif

