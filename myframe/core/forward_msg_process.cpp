#include "forward_msg_process.h"
#include "forward_obj_container.h"


size_t forward_msg_process::process_recv_buf(char *buf, size_t len)
{
    //PDEBUG("recv buf %d\n", len);
    //size_t ret = 0;
    size_t left_len = len;
    while(left_len > 0)
    {
        RECV_MSG_STATUS status = RECV_MSG_HEAD;
        _head_len = sizeof(int);
        size_t msg_body_len = 0;
        if (status == RECV_MSG_HEAD)
        {
            if (left_len > _head_len)
            {

                int *p_len = (int *)buf;
                msg_body_len = *p_len;

                LOG_DEBUG("left_len[%d] _head_len[%d] msg_body_len[%d]\n", left_len, _head_len, msg_body_len);
                status = RECV_MSG_BODY;
            }
            else
            {
                break;
            }
        }

        if (status == RECV_MSG_BODY)
        {
            if (left_len >= _head_len + msg_body_len) {
                process_s(buf, _head_len + msg_body_len);

                left_len -= (_head_len + msg_body_len);
                buf = buf + _head_len + msg_body_len;
            } else {
                break;
            } 
        }				
    }
    //PDEBUG("len[%d]\n", len - left_len);

    return len - left_len;
}	


size_t forward_msg_process::process_s(char *buf, size_t len)
{
    put_msg(buf, len);
    return len;
}

void forward_msg_process::put_msg(char *buf, size_t len)
{
    if (!buf || len < _head_len){
        //LOG_WARN
        return;
    }

    if (!_container){
        return;
    }


    PassMsg pass_msg;
    pass_msg.ParseFromArray(buf + _head_len, len - _head_len);

    uint32_t channelid = _container->get_dest_channelid(pass_msg.dst_id().thread_index());

    if (channelid) {
        write(channelid, buf, len);
        //PDEBUG("write channelid[%d] len[%d]\n", channelid, len);
    }
}

void forward_msg_process::set_forward_container(forward_obj_container * container)
{
    _container = container;
}



