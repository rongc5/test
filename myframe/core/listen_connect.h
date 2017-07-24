#ifndef __LISTEN_CONNECT_H__
#define __LISTEN_CONNECT_H__

#include "common_def.h"


class base_net_obj;
class listen_data_process;
class listen_connect:public base_net_obj
{
	public:
		listen_connect(int fd);

		virtual ~listen_connect();

		virtual void event_process(int events);
        virtual int real_net_process();

        void set_process(listen_data_process *p);

        listen_data_process * get_process();


	private:
        listen_data_process *_process;
};


#endif

