#ifndef _CHANNEL_MSG_PROCESS_H_
#define _CHANNEL_MSG_PROCESS_H_

#include "common_epoll.h"
#include "net_obj.h"
#include "common_def.h"
#include "log_helper.h"
#include "base_msg_process.h"
#include "base_net_container.h"
#include "passing_msg_thread.h"


class passing_msg_process:public base_msg_process
{
    public:
    passing_msg_process(void *p):base_msg_process(p), _thread(NULL)
    {
    }

        virtual ~passing_msg_process()
        {
        }	

        virtual size_t process_recv_buf(char *buf, size_t len)
        {
            LOG_DEBUG("recv buf %d", len);
            PDEBUG("recv buf %d", len);
            //size_t ret = 0;
            size_t left_len = len;
            while(left_len > 0)
            {
                RECV_MSG_STATUS status = RECV_MSG_HEAD;
                size_t msg_body_len = 0;
                if (status == RECV_MSG_HEAD)
                {
                    if (left_len > _head_len)
                    {
                        const char *ptr = "";

                        if (_head_len >= MSG_HEAD_BODY_LENTH_LEN) 
                            ptr = buf + _head_len - MSG_HEAD_BODY_LENTH_LEN;
                        else {
                            ptr = buf;
                        }

                        msg_body_len = ntohl(*ptr);

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


        size_t process_s(char *buf, size_t len)
        {
            put_msg(buf, len);
            return len;
        }

        void put_msg(char *buf, size_t len)
        {
            PDEBUG("step 888888888888888");
            if (!buf || len < sizeof(_pass_msg_t)){
                //LOG_WARN
                return;
            }
            PDEBUG("step 1111111111111119");

            _pass_msg_t * ptr = (_pass_msg_t *)buf;

            if (!_thread){
                return;
            }

            common_thread * dest_thread = _thread->get_dest_thread(ptr->_dst_obj._thread_id);
            if (dest_thread && (dest_thread->get_passing_type() & PASSING_ACCEPT_IN)) {
                write(dest_thread->get_channelid(), buf, len);
            }
        }

        void set_passing_thread(passing_msg_thread * thread)
        {
            _thread = thread;
        }

        passing_msg_thread * get_passing_thread()
        {
            return _thread;
        }

    protected:
        passing_msg_thread * _thread;

};

#endif

