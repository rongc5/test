#ifndef _BASE_DATA_PROCESS_H_
#define _BASE_DATA_PROCESS_H_

#include "common_def.h"

class base_net_obj;
class base_data_process
{
    public:
        base_data_process(base_net_obj *p);

        virtual ~base_data_process();

        virtual void peer_close();

        virtual string *get_send_buf();

        virtual void reset();

        virtual void set_para();

        virtual void on_connect_comming();

        virtual size_t process_recv_buf(const char *buf, size_t len);

        virtual bool process_recv_msg(ObjId & id, normal_msg * p_msg);

        void add_timer(timer_msg & t_msg);

        virtual bool handle_timeout(timer_msg & t_msg);

        void put_send_buf(string * str);

        base_net_obj * get_base_net();

    protected:
        void clear_send_list();

    protected:
        base_net_obj *_p_connect;
        list<string*> _send_list;
};

#endif

