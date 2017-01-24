#include "common_epoll.h"
#include "net_obj.h"
#include "base_net_container.h"
#include "common_net_thread.h"
#include "channel_msg_process.h"



size_t channel_msg_process::process_recv_buf(char *buf, size_t len)
{
    PDEBUG("recv buf [%d] tid [%lu] step1\n", len, pthread_self());
    //size_t ret = 0;
    size_t left_len = len;
    while(left_len > 0)
    {
        PDEBUG("recv buf [%d] tid [%lu] left_len[%d] step2\n", len, pthread_self(), left_len);
        RECV_MSG_STATUS status = RECV_MSG_HEAD;
        size_t msg_body_len = 0;
        _head_len = sizeof(_pass_msg_t);
        if (status == RECV_MSG_HEAD)
        {
            if (left_len > _head_len)
            {
                _pass_msg_t * ptr = (_pass_msg_t *) buf;

                PDEBUG("recv buf [%d] tid [%lu] left_len[%d] _head_len[%d] step3\n", len, pthread_self(), left_len, _head_len);
                msg_body_len = ptr->body_len;

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

                left_len -= _head_len + msg_body_len;
                buf = buf + left_len;
            } else {
                break;
            } 
        }				
    }

    return len - left_len;
}	


size_t channel_msg_process::process_s(char *buf, size_t len)
{
    if (!buf || !len || len < sizeof(_pass_msg_t)) {
        return 0;
    } 

    _pass_msg_t *head = (_pass_msg_t *)buf;

    switch (head->_op)
    {
        case ADD_NEW_SOCEKT:
            {
                int * p = (int *)(buf + sizeof(head));
                common_net_thread<channel_msg_process> *net_thread = dynamic_cast<common_net_thread<channel_msg_process> * >(_thread);
                if (net_thread){
                    net_thread->gen_connect(*p);
                }
            }

            break;
        default:
            put_msg(buf, len);
    }

    return len;
}

void channel_msg_process::put_msg(char * buf, size_t len)
{
    if (!buf || len < sizeof(_pass_msg_t)){
        //LOG_WARN
        return;
    }

    _pass_msg_t * ptr = (_pass_msg_t *)buf;

    base_net_container * net_container = _p_connect->get_net_container();
    if (!net_container) {
        return;
    }
    NET_OBJ * net_obj = dynamic_cast<NET_OBJ *>(net_container->find(&ptr->_dst_obj));
    if (net_obj) {
        net_obj->process_recv_buf(buf, len);
    }
}

void channel_msg_process::set_common_thread(common_thread *thread)
{
    _thread = thread;
}

common_thread* channel_msg_process::get_common_thread()
{
    return _thread;
}




