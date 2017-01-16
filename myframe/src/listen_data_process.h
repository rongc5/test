#ifndef __LISTEN_DATA_PROCESS_H__
#define __LISTEN_DATA_PROCESS_H__

#include "common_msg_process.h"
#include "listen_thread.h"

class listen_data_process
{
	public:
		listen_data_process(void *p):_thread(NULL)
		{
            _p_connect = (base_net_obj*)p;
		}
		
		virtual ~listen_data_process(){}

		virtual size_t process(int fd)
        {

            if (_thread){
                
                string tmp_str;
                _pass_msg_t head;
                head._dst_obj._thread_id = _thread->get_worker_id();
                head._src_obj._thread_id = _p_connect->_p_connect->get_id();
                head.len = sizeof(add_new_socket);

                add_new_socket add_msg;
                add_msg._op = ADD_NEW_SOCEKT;
                add_msg.fd = fd;

                memcpy(tmp_str.c_str(), &head, sizeof(head));
                memcpy(tmp_str.c_str()+tmp_str.length(), &add_new_socket, sizeof(add_new_socket));

                write(_thread->get_channelid(), tmp_str.c_str(), tmp_str.length());
            }
        }

        void set_thread(listen_thread * thread)
        {
            _thread = thread;    
        }
	protected:	
        base_net_obj * _p_connect;
        listen_thread * _thread;
};
#endif

