#ifndef __BASE_TIMER_PROCESS_H__
#define __BASE_TIMER_PROCESS_H__

#include "base_def.h"

class base_timer_process
{   
    public:

        base_timer_process()
        {   
            _timer_type = 0;
            _time_length = 0;
            _reach_time = 0;
        }

        virtual ~base_timer_process();

        virtual void handle_timeout();

        void init(uint32_t timer_type, uint64_t time_length);

        void set_reach_time(uint64_t reach_time);

        uint64_t get_reach_time();

        uint64_t get_time_length();

        uint32_t get_timer_type();

    protected:

        uint32_t _timer_type;
        uint64_t _time_length;
        uint64_t _reach_time;
};


#endif
