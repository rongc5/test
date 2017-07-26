#include "common_timer_process.h"
#include "base_net_container.h"


common_timer_process::common_timer_process(base_net_container * net_container, ObjId id_str)
{
    _net_container = net_container;
    _id_str = id_str;
}

void common_timer_process::handle_timeout()
{
    if (_net_container) {
        base_net_obj * net_obj = _net_container->find(&_id_str);
        if (net_obj) {
            net_obj->handle_timeout(get_timer_type());
        }
    }
}


