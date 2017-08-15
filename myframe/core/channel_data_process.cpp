#include "channel_data_process.h"
#include "base_net_thread.h"
#include "base_net_obj.h"
#include "base_connect.h"


channel_data_process::channel_data_process(base_connect *p):base_data_process(p)
{
}

size_t channel_data_process::process_recv_buf(const char *buf, size_t len)
{
    thread_lock lock(&_mutex);

    int i = 0;
    deque<normal_obj_msg >::iterator it;
    for (it = _queue.begin(); it != _queue.end();){
       if (it->p_msg->_msg_op == MSG_CONNECT) {
           base_net_obj * p_connect = dynamic_cast<base_net_obj *> (it->p_msg); 
           if (p_connect) {
                p_connect->set_net_container(_p_connect->get_net_container());
            } else {
                REC_OBJ<normal_msg> rc(it->p_msg);
            }
       } else {
            if (it->_id._id) {
                _p_connect->get_net_container()->put_msg(it->_id, it->p_msg);
            } else {
                _p_connect->get_net_container()->get_net_thread()->handle_msg(it->p_msg);
            }
       } 

        it = _queue.erase(it);
        i++;
    }

     size_t r_len =  i * sizeof(CHANNEL_MSG_TAG);
     if (r_len > len)
         r_len = len;

    return r_len;
}


bool channel_data_process::process_recv_msg(ObjId & id, normal_msg * p_msg)
{
    if (!p_msg) {
        return 1;
    }

    thread_lock lock(&_mutex);
    normal_obj_msg nbj_msg;
    nbj_msg.p_msg = p_msg;
    nbj_msg._id = id;
    _queue.push_back(nbj_msg);

    return true;
}

