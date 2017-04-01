#include "base_connect.h"
#include "base_net_thread.h"


base_connect::base_connect(base_net_thread * thread):_thread(thread)
{   
    _id_str = thread->gen_id_str();
}

int base_connect::destroy()
{
    _thread->destory_connect(_id_str); 
    return 0;
}

const ObjId & base_connect::get_id()
{
    return _id_str;
}

