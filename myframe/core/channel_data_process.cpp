#include "channel_data_process.h"
#include "base_net_thread.h"
#include "base_net_obj.h"
#include "base_connect.h"
#include "log_helper.h"
#include "base_net_obj.h"
#include "common_util.h"

channel_data_process::channel_data_process(base_net_obj *p):base_data_process(p)
{
}

size_t channel_data_process::process_recv_buf(const char *buf, size_t len)
{
    LOG_DEBUG("start1");
    size_t k = len /sizeof(CHANNEL_MSG_TAG);
    std::lock_guard<std::mutex> lck (_mutex);

    size_t i = 0;
    std::deque<normal_obj_msg >::iterator it;
    for (it = _queue.begin(); it != _queue.end() && i<k;){
        if (it->p_msg->_msg_op == MSG_CONNECT) {
            std::shared_ptr<base_net_obj>  p_connect = std::dynamic_pointer_cast<base_net_obj> (it->p_msg); 
            if (p_connect.use_count()) {
                p_connect->set_net_container(_p_connect->get_net_container());
            } 
        } else {
            if (it->_id._id > OBJ_ID_BEGIN) {
                _p_connect->get_net_container()->put_msg(it->_id, it->p_msg);
            } else {
                _p_connect->get_net_container()->get_net_thread()->handle_msg(it->p_msg);
            }
        } 

        it = _queue.erase(it);
        i++;
    }

     k =  i * sizeof(CHANNEL_MSG_TAG);

    return k;
}


bool channel_data_process::process_recv_msg(ObjId & id, std::shared_ptr<normal_msg> & p_msg)
{
    std::lock_guard<std::mutex> lck (_mutex);
    normal_obj_msg nbj_msg;
    nbj_msg.p_msg = p_msg;
    nbj_msg._id = id;
    _queue.push_back(nbj_msg);

    return true;
}

