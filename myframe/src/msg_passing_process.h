#ifndef _MSG_PASSING_PROCESS_H_
#define _MSG_PASSING_PROCESS_H_

#include "common_epoll.h"
#include "net_obj.h"
#include "common_def.h"

template<class DATA_PROCESS>
class msg_passing_process
{
    public:
        msg_passing_process(void *p)
        {
            //_p_cur_send_msg = NULL;
            clear_send_list();
            _p_connect = (NET_OBJ*)p;
        }

        virtual ~msg_passing_process()
        {
            clear_send_list();
        }	

        size_t process_recv_buf(char *buf, size_t len)
        {
            LOG_DEBUG("recv buf %d", len);
            //size_t ret = 0;
            size_t left_len = len;
            while(left_len > 0)
            {
                RECV_MSG_STATUS status = RECV_MSG_HEAD;
                size_t msg_body_len = 0;
                uint32_t head_len = sizeof(_pass_msg_t);
                if (status == RECV_MSG_HEAD)
                {
                    if (left_len > head_len)
                    {
                        char *ptr = "";

                        if (head_len >= MSG_HEAD_BODY_LENTH_LEN) 
                            ptr = buf + head_len - MSG_HEAD_BODY_LENTH_LEN;
                        else {
                            ptr = buf;
                        }

                        msg_body_len = ntohl(*ptr);

                        _status = RECV_MSG_BODY;
                    }
                    else
                    {
                        break;
                    }
                }

                if (status == RECV_MSG_BODY)
                {
                    if (left_len >= head_len + msg_body_len) {

                        process_s(buf, head_len +msg_body_len);

                        left_len -= head_len + msg_body_len;
                        buf = buf + left_len;
                    } else {
                        break;
                    } 
                }				
            }

            return len - left_len;
        }	

        void process_s(char *buf, size_t len)
        {
            if (len < sizeof(_pass_msg_t) || !buf) {
                return ;
            }

            _pass_msg_t * ptr = (_pass_msg_t *) buf;
            
            int id  = _msg_passing_thread::find_channelid(ptr->_dst_obj._thread_id);
            if (id != -1){
                write(id, buf);
                delete buf;
            }

        }

        string *get_send_buf()
        {
            string *ret =  get_send_msg();
            return ret;
        }

        void handle_timeout(const uint32_t timer_type)
        {
        }

        void reset()
        {
            clear_send_list();
        }

        virtual void routine()
        {
        }

        NET_OBJ* get_connect()
        {
            return _p_connect;
        }

        connect_info *gen_connect_info()
        {
            connect_info *p_info = new ku_connect_info();
            return p_info;
        }

        void peer_close()
        {
        }

        void set_para()
        {		   
        }

        void on_connect_comming()
        {
        }

    private:

        void clear_send_list()
        {			
            for (list<string*>::iterator itr = _send_list.begin(); itr != _send_list.end(); ++itr)
            {
                delete *itr;
            }
            _send_list.clear();
        }

        string *get_send_msg()
        {
            if (_send_list.begin() == _send_list.end())
                return NULL;

            string *p = *(_send_list.begin());
            _send_list.erase(_send_list.begin());
            return p;
        }

    private:		
        list<string*> _send_list;
        NET_OBJ *_p_connect;
};
#endif

