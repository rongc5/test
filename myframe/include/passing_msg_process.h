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
            PDEBUG("recv buf %d\n", len);
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

                        PDEBUG("left_len[%d] _head_len[%d] msg_body_len[%d]\n", left_len, _head_len, msg_body_len);
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
            PDEBUG("len[%d]\n", len - left_len);

            return len - left_len;
        }	


        size_t process_s(char *buf, size_t len)
        {
            put_msg(buf, len);
            return len;
        }

        void put_msg(char *buf, size_t len)
        {
            if (!buf || len < _head_len){
                //LOG_WARN
                return;
            }

            if (!_thread){
                return;
            }

            
             PassMsg pass_msg;
             pass_msg.ParseFromArray(buf + _head_len, len - _head_len);

            common_thread * dest_thread = _thread->get_dest_thread(pass_msg.dst_id().thread_index());
            if (dest_thread) {
                PDEBUG("%d %d %d\n",dest_thread->get_thread_index(), dest_thread->get_passing_type(), dest_thread->get_passing_type() & PASSING_ACCEPT_IN);
            }
            
            int channelid = 0;
            if (dest_thread){
                channelid = _thread->get_dest_channelid(dest_thread->get_thread_index());
            }

            if (dest_thread && (dest_thread->get_passing_type() & PASSING_ACCEPT_IN) && channelid) {
                write(channelid, buf, len);
                PDEBUG("write channelid[%d] len[%d]\n", channelid, len);
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

