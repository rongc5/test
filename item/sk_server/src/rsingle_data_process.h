#ifndef __RSINGLE_DATA_PROCESS_H__
#define __RSINGLE_DATA_PROCESS_H__

#include "common_def.h"
#include "http_base_process.h"
#include "http_base_data_process.h"
#include "base_net_obj.h"
#include "sk_def.h"

class common_obj_container;
class rsingle_data_process: public http_base_data_process
{
    public:
        rsingle_data_process(http_base_process * _p_process);

        virtual ~rsingle_data_process()
        {
        }

        virtual std::string *get_send_body(int &result);

        virtual void header_recv_finish();

        virtual void msg_recv_finish();

        virtual std::string * get_send_head();

        virtual size_t process_recv_body(const char *buf, size_t len, int& result);

        void set_url_info(url_info & u_info);

        url_info & get_url_info();

        static void gen_net_obj(std::string id, common_obj_container * net_container, std::map<std::string, std::string> & headers);

        virtual void handle_timeout(std::shared_ptr<timer_msg> & t_msg);

        virtual void destroy();

        void set_id(std::string id);

        static void single_index_reset();

        static void update_all_index();
        static void update_real_index();
        static void update_history_index();
        static void update_sum_index();

        static void single_idle_current();

        static int get_single_index(const std::string &id, uint32_t index);

        static int get_single_diff2(std::deque<std::shared_ptr<single_vec> > & st, uint32_t index);

    protected:
        std::string _recv_buf;
        std::string _id;
        url_info _url_info;
        bool _is_ok;

};




#endif
