#ifndef __WS_SER_THREAD_H__
#define __WS_SER_THREAD_H__

#include "common_def.h"
#include "base_net_thread.h"

class base_connect;
class ws_ser_thread:public base_net_thread
{
    public:
        ws_ser_thread(){
            _ws_ser_thread_vec.push_back(this);
        };

        virtual ~ws_ser_thread(){
        };

        virtual bool handle_msg(base_passing_msg * msg);
        
        static void put_msg(base_passing_msg * msg);

    protected:
        static vector<ws_ser_thread *> _ws_ser_thread_vec;

};

#endif
