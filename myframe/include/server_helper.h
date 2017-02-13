#ifndef __SERVER_HELPER_H__
#define __SERVER_HELPER_H__


#include "common_thread.h"



    typedef int (* callback_proc)();


		struct server_info_t {
		
			 int _thread_num; 

       int _con_timeout;
       int _read_timeout;

       unsigned short _listen_port;
       
       string _ip;
       
       server_info_t & operator = (const server_info_t & ser_t); 			
		};


    class server_helper {
    
        public:
				server_helper();
				~server_helper();
				
				void run();
				void init(const server_info_t & ser);

        private:  
						common_thread _listen_thread;
						vector<common_thread *> _work_thread_mgr; 
						server_info_t _server_info;

    };




#endif
