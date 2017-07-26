#ifndef __COMMON_TIMER_PROCESS_H__
#define __COMMON_TIMER_PROCESS_H__

#include "base_def.h"
#include "base_timer_process.h"
#include "common_def.h"

class base_net_container;
class common_timer_process: public base_timer_process
{   
    public:

        common_timer_process(base_net_container * net_container, ObjId id_str);

        virtual ~common_timer_process(){}

        virtual void handle_timeout();


    protected:
        base_net_container * _net_container;
        ObjId _id_str;
};


#endif
