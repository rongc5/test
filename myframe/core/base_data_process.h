#ifndef _BASE_DATA_PROCESS_H_
#define _BASE_DATA_PROCESS_H_

#include "common_epoll.h"
#include "net_obj.h"
#include "common_def.h"
#include "base_net_thread.h"

class base_net_thread;
class base_data_process
{
    public:
        base_data_process(void *p)
        {
            _p_connect = (NET_OBJ*)p;
            _thread = NULL;
        }

        virtual ~base_data_process()
        {

        }	

        virtual void peer_close()
        {
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

        virtual void set_para()
        {
        }

        virtual void on_connect_comming()
        {
        }

        virtual size_t process_recv_buf(char *buf, size_t len);

        size_t process_s(pass_msg * p_msg);


        void set_base_net_thread(base_net_thread *thread);

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
        base_net_thread * _thread;
        NET_OBJ *_p_connect;
        list<string*> _send_list;
};

#endif

