#ifndef __CHANNEL_DATA_PROCESS_H__
#define __CHANNEL_DATA_PROCESS_H__

#include "common_msg_process.h"

class channel_data_process
{
	public:
		channel_data_process(void *p)
		{
			_p_msg_process = (channel_msg_process<channel_data_process>*)p;			
		}
		
		virtual ~channel_data_process(){}

		virtual size_t process_recv_buf(char *buf, size_t len)
        {

        }

        static channel_data_process* gen_process(void *p) 
        {   
            channel_data_process *p_tmp = new channel_data_process(p);
            return p_tmp;
        }   

	protected:	
		channel_msg_process<channel_data_process> *_p_msg_process;
};
#endif

