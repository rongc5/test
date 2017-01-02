#include "common_thread.h"

namespace MZFRAME {


    common_thread::common_thread()
    {
        _channel_id = 0;
    }


    void common_thread::*run()
    {
        while(get_run_flag())
        {
            obj_process();
        }
    }
}
