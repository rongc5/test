#ifndef __BASE_TIMER_PROCESS_H__
#define __BASE_TIMER_PROCESS_H__

class base_timer_process
{   
    public:

    uint32_t _timer_type;
    base_timer_process()
    {   
        _timer_type = 0;
    }

    virtual ~base_timer_process();

    virtual void handle_timeout(const uint32_t timer_type);
};


#endif
