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

            virtual size_t process_recv_buf(char *buf, size_t len) = 0;

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


            virtual size_t process_s(char *buf, size_t len) = 0;

            virtual void set_para()
            {
            }

            virtual void on_connect_comming()
            {
            }

            virtual void put_msg(char *buf, size_t len) = 0;

            void set_head_len(size_t head_len)
            {
                _head_len = head_len;
            }

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

