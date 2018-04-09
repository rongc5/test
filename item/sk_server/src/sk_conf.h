#ifndef __CORE_CONF_H__
#define __CORE_CONF_H__

#include "base_reload.h"
#include "log_helper.h"

class sk_conf: public reload_inf
{
    public:
        sk_conf(const char * sk_conf);

        std::string ua_path;
        std::string ua_file;

        std::string id_path;
        std::string id_file;
        
        std::string financie_path;
        std::string financie_file;

        std::string ban_path;
        std::string ban_file;

        std::string local_strategy_path;
        std::string local_strategy_file;

        std::string history_single_path;
        std::string history_single_file;

        std::string history_quotation_path;
        std::string history_quotation_file;


        uint32_t http_req_thread_num;

        uint32_t http_server_port;
        uint32_t http_server_thread_num;

        std::string ip_deny_path;
        uint32_t ip_deny_num;

        std::string dump_dir;
        uint32_t reload_second;

        LogType log_type;

        std::string real_morning_stime;
        std::string real_morning_etime;

        std::string real_afternoon_stime;
        std::string real_afternoon_etime;

    public:

        virtual int load();
        virtual int reload();
        virtual bool need_reload();
        virtual int dump();
        virtual int destroy();

    private:
        std::string _filename;
};



#endif
