#ifndef __CORE_CONF_H__
#define __CORE_CONF_H__

#include "base_reload.h"
#include "log_helper.h"
#include "strategy_conf.h"

class sk_conf: public reload_inf
{
    public:
        sk_conf(const char * sk_conf);

        std::string strategy_path;
        std::string strategy_file;

        std::string http_server_host;

        uint32_t http_server_port;
        uint32_t http_server_thread_num;

        //std::string ip_deny_path;

        std::string dump_dir;

        LogType log_type;

        reload_mgr<strategy_conf> * _strategy;

    public:

        virtual int load();
        virtual int reload();
        virtual bool need_reload();
        virtual int dump();
        virtual int destroy();
        int destroy_idle();

    private:
        std::string _filename;
};



#endif
