#ifndef __BASE_DATA_PROCESS_H__
#define __BASE_DATA_PROCESS_H__

#include "base_msg_process.h"

class base_data_process
{
	public:
		base_data_process(void *p)
		{
			_p_msg_process = (base_msg_process<base_data_process>*)p;			
		}
		
		virtual ~base_data_process(){}

		virtual size_t process_recv_buf(char *buf, size_t len);

		virtual void reset()
        {   
            _s_buf.clear();
        }   

        static base_data_process* gen_process(void *p) 
        {   
            base_data_process *p_tmp = new base_data_process(p);
            return p_tmp;
        }   

        virtual string get_head_buf()
        {
			string ret;
			return ret;
        }


        void  on_connect_comming()
        {

        }

        virtual uint32_t get_head_len() = 0;

	public:
		string _s_buf;

    protected:
        virtual void put_msg(string * p_msg);
	private:	

		base_msg_process<base_data_process> *_p_msg_process;
};
#endif

