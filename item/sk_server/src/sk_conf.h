#ifndef __CORE_CONF_H__
#define __CORE_CONF_H__

#include "base_reload.h"

class sk_conf: public reload_inf
{
    public:
        sk_conf(const char * sk_conf);

        string ua_path;

        string id_path;
        uint32_t id_num;
        
        string financie_path;
        uint32_t financie_num;

        string ban_path;
        uint32_t ban_num;

        string local_strategy_path;
        uint32_t http_req_thread_num;

        uint32_t http_server_port;
        uint32_t http_server_thread_num;

        string ip_deny_path;
        uint32_t ip_deny_num;

        string dump_dir;
        uint32_t reload_second;

        LogType log_type;

    public:

        virtual int load();
        virtual int reload();
        virtual bool need_reload();
        virtual int dump();
        virtual int destroy();

    private:
        string _filename;
};



#endif
