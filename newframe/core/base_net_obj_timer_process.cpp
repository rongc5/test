#include "base_net_obj_timer_process.h"
#include "base_net_thread.h"

base_net_obj_timer_process::base_net_obj_timer_process(ObjId  id, base_net_thread *thread)
{
    _thread = thread;
    _id = id;
}

base_net_obj_timer_process::base_net_obj_timer_process()
{
    _thread = NULL;
}

void base_net_obj_timer_process::handle_timeout(const uint32_t timer_type)
{
    if (_thread) {
        base_connect * b_cn = _thread->get_connect(_id);
        if (b_cn) {
            b_cn->handle_timeout(timer_type);
        }
    }
}





