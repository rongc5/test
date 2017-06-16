#ifndef _BASE_DATA_PROCESS_H_
#define _BASE_DATA_PROCESS_H_

#include "common_def.h"

class base_data_process
{
    public:
        base_data_process();

        virtual ~base_data_process();

        virtual void err_close();

        string *get_send_buf();

        virtual void reset();

        virtual void set_para();

        virtual void on_connect_comming();

        virtual size_t process_recv_buf(char *buf, size_t len);

        virtual size_t process_recv_msg(base_passing_msg * p_msg);

        virtual void notice_send();

        virtual void handle_timeout(const uint32_t timer_type);
};

#endif

