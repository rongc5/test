#include "base_connect.h"
#include "base_net_thread.h"


base_connect::base_connect(base_net_thread * thread):_thread(thread)
{   
    _id_str = thread->gen_id_str();
}

int base_connect::destroy()
{
    _thread->destory_connect(_id_str); 
    delete this;
    return 0;
}

const ObjId & base_connect::get_id()
{
    return _id_str;
}

void base_connect::recv_passing_msg(base_passing_msg * p_msg)
{
    REC_OBJ<base_passing_msg> rc(p_msg); 
}
