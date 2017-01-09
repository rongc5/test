#ifndef __PASSING_DATA_PROCESS_H__
#define __PASSING_DATA_PROCESS_H__

#include "common_msg_process.h"

class passing_data_process
{
	public:
		passing_data_process(void *p)
		{
			_p_msg_process = (common_msg_process<base_data_process>*)p;			
		}
		
		virtual ~passing_data_process(){}

		virtual size_t process_recv_buf(char *buf, size_t len)
        {
            if (!buf || !len) {
                return -1;
            }

            string * tmp_str = new string(buf, len);
            _p_msg_process->put_msg(tmp_str);            
        }

        static passing_data_process* gen_process(void *p) 
        {   
            passing_data_process *p_tmp = new passing_data_process(p);
            return p_tmp;
        }   

	protected:	
		passing_msg_process<passing_data_process> *_p_msg_process;
};
#endif

