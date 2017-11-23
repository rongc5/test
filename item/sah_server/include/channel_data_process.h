#ifndef _CHANNEL_DATA_PROCESS_H_
#define _CHANNEL_DATA_PROCESS_H_

#include "common_def.h"
#include "thread_helper.h"

class base_net_obj;
class channel_data_process
{
    public:
        channel_data_process(base_net_obj *p);

        ~channel_data_process()
        {
            deque<normal_obj_msg >::iterator it;
            for (it = _queue.begin(); it != _queue.end(); it++) {
                delete it->p_msg;
            }
            
            clear_send_list();
        }	
        
        
          void peer_close();

        string *get_send_buf();

         void reset();

         void set_para();

         void on_connect_comming();
          

        size_t process_recv_buf(const char *buf, size_t len);

        bool process_recv_msg(ObjId & id, normal_msg * p_msg);
    
			
		  void put_send_buf(string * str);

        base_net_obj * get_base_net();
			
	private:
		void clear_send_list();
	
    protected:
        thread_mutex_t _mutex;
        deque<normal_obj_msg > _queue;
          
          base_net_obj *_p_connect;
          list<string*> _send_list;
};

#endif

