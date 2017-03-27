#include "test_data_process.h"
#include "test_net_thread.h"
#include "base_singleton.h"



size_t test_data_process::process_recv(pass_msg * msg)
{
    REC_OBJ<pass_msg> rec(msg);


    if (msg->_src_id == _p_connect->get_id()) {
        LOG_DEBUG("_p_connect->get_id thread_index[%d]", _p_connect->get_id()._thread_index);
        pass_msg * p_msg = new pass_msg();
        p_msg->_src_id = _p_connect->get_id();
        p_msg->_dest_id._thread_index = base_singleton<test_net_thread>::get_instance()->get_thread_index();;
        LOG_DEBUG("dest thread_index[%d]", p_msg->_dest_id._thread_index);
        p_msg->_p_op = PASS_NEW_MSG;
        p_msg->_flag = 1;

        char * ptr = new char[SIZE_LEN_128];
        recv_msg * r_msg = new recv_msg();
        r_msg->_ptr = ptr;
        p_msg->p_msg = r_msg;


        r_msg->_len = snprintf(ptr, SIZE_LEN_128, "I am from %d", _p_connect->get_id()._thread_index);

        base_net_thread::passing_msg(p_msg);

        return 0;
    }

    if (msg->_p_op == PASS_NEW_MSG){
        recv_msg * r_msg = dynamic_cast<recv_msg *> (msg->p_msg);
        string * str = new string(r_msg->_ptr);
        process_send(str);
    }


    return 0;
}



