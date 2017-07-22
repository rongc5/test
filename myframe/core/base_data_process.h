#ifndef _BASE_DATA_PROCESS_H_
#define _BASE_DATA_PROCESS_H_

#include "common_def.h"

class base_connect;
class base_data_process
{
    public:
        base_data_process(base_connect *p);

        virtual ~base_data_process();

        virtual void peer_close();

        virtual string *get_send_buf();

        virtual void reset();

        virtual void set_para();

        virtual void on_connect_comming();

        virtual size_t process_recv_buf(char *buf, size_t len);

        virtual bool process_recv_msg(ObjId & id, normal_msg * p_msg);

        virtual void handle_timeout(const uint32_t timer_type);

        void put_send_buf(string * str);

        base_connect * get_base_connect();

    protected:
        void clear_send_list();

    protected:
        base_connect *_p_connect;
        list<string*> _send_list;
};

#endif

