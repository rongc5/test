#ifndef __BASE_TIMER_PROCESS_H__
#define __BASE_TIMER_PROCESS_H__

struct base_timer_process
{   
    uint32_t _timer_type;
    base_timer_process()
    {   
        _timer_type = 0;
    }

    virtual void handle_timeout(const uint32_t timer_type);
};


#endif
