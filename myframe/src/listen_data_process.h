#ifndef __LISTEN_DATA_PROCESS_H__
#define __LISTEN_DATA_PROCESS_H__

#include "common_msg_process.h"

class listen_data_process
{
	public:
		listen_data_process(void *p)
		{
			_p_msg_process = (common_msg_process<listen_data_process>*)p;			
		}
		
		virtual ~listen_data_process(){}

		virtual size_t process_recv_buf(char *buf, size_t len)
        {
        }

        static listen_data_process* gen_process(void *p) 
        {   
            listen_data_process *p_tmp = new listen_data_process(p);
            return p_tmp;
        }   

	protected:	
		common_msg_process<listen_data_process> *_p_msg_process;
};
#endif

