#include "common_thread.h"
#include "base_thread.h"


common_thread::common_thread()
{
    _channel_id = 0;
    set_passing_type(PASSING_ACCEPT_IN);
}


void * common_thread::run()
{
    while(get_run_flag())
    {
        obj_process();
    }

    return NULL;
}
