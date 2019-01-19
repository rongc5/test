#ifndef _BASE_DATA_PROCESS_H_
#define _BASE_DATA_PROCESS_H_

#include "common_util.h"

class base_net_obj;
class base_data_process
{
    public:
        base_data_process(std::shared_ptr<base_net_obj> p);

        virtual ~base_data_process();

        virtual void peer_close();

        virtual std::string *get_send_buf();

        virtual void reset();

        virtual size_t process_recv_buf(const char *buf, size_t len);

        virtual void handle_msg(std::shared_ptr<normal_msg> & p_msg);

        void add_timer(std::shared_ptr<timer_msg> & t_msg);

        virtual void handle_timeout(std::shared_ptr<timer_msg> & t_msg);

        void put_send_buf(std::string * str);

        std::shared_ptr<base_net_obj>  get_base_net();

        virtual void destroy();

    protected:
        void clear_send_list();

    protected:
        std::weak_ptr<base_net_obj> _p_connect;
        std::list<std::string*> _send_list;
};

#endif

