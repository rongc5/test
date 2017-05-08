#ifndef __HTTP_CLIENT_THREAD_H__
#define __HTTP_CLIENT_THREAD_H__

#include "base_net_thread.h"

class http_client_thread:public base_net_thread
{
    public:

        http_client_thread();
        virtual ~http_client_thread(){}

        virtual bool handle_msg(base_passing_msg * msg);
        
           static void put_msg(base_passing_msg * msg);

    protected:

        static vector<http_client_thread *>  _http_thread_vec;

};



#endif
