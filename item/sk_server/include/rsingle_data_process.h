#ifndef __RSINGLE_DATA_PROCESS_H__
#define __RSINGLE_DATA_PROCESS_H__

#include "common_def.h"
#include "http_base_process.h"
#include "http_base_data_process.h"
#include "base_net_obj.h"
#include "sk_def.h"
#include "curl_req.h"

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

        static int load_from_curl(std::string id, common_obj_container * net_container, std::map<std::string, std::string> & headers, curl_req & cur);

        static int parse_single(std::string id, std::string rec_str);

        virtual void handle_timeout(std::shared_ptr<timer_msg> & t_msg);

        virtual void destroy();

        void set_id(std::string id);

        static std::shared_ptr<single_vec> get_rsingle(std::string id, std::shared_ptr<single_vec> & tmp_single);

        static int get_single_index(const std::string &id, uint32_t index);
        static void gen_destroy_msg(const std::string &id);
        static void gen_response_req_msg(const std::string &id, std::shared_ptr<single_vec> st);

    protected:
        std::string _recv_buf;
        std::string _id;
        url_info _url_info;
        bool _is_ok;

};




#endif
