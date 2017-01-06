#ifndef _BASE_MSG_PROCESS_H_
#define _BASE_MSG_PROCESS_H_

#include "common_epoll.h"
#include "net_obj.h"
#include "common_def.h"

class base_msg_process
{
    public:
        base_msg_process(void *p)
        {
            //_p_cur_send_msg = NULL;
            clear_send_list();
            _p_connect = (NET_OBJ*)p;
            _head_len = 0;
        }

        virtual ~base_msg_process()
        {
            clear_send_list();
        }	

        virtual size_t process_recv_buf(char *buf, size_t len)
        {
            LOG_DEBUG("recv buf %d", len);
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
                        char *ptr = "";

                        if (_head_len >= MSG_HEAD_BODY_LENTH_LEN) 
                            ptr = buf + _head_len - MSG_HEAD_BODY_LENTH_LEN;
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
                    if (left_len >= _head_len->get_head_len() + msg_body_len) {
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

        virtual int process_send_buf(string * buf)
        {
            _send_list.push_back(tmp_str);

            return 0;
        }

        string *get_send_buf()
        {
            string *ret =  get_send_msg();
            return ret;
        }

        virtual void reset()
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

        virtual void peer_close()
        {
        }


        virtual size_t process_s(char *buf, size_t len) = 0

        virtual void set_para()
        {
        }

        virtual void on_connect_comming()
        {
        }

        virtual void put_msg(string *p_msg) = 0;

    protected:

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

    protected:		
        size_t _head_len;
        list<string*> _send_list;
        NET_OBJ *_p_connect;
};
#endif

