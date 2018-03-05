#include "sk_conf.h"
#include "common_cfgparser.h"

sk_conf::sk_conf(const char * sk_conf)
{
    id_num = 2000;

    http_req_thread_num = 3;
    
    http_server_port = 8080;
    http_server_thread_num = 3;

    ip_deny_num = 100;

    reload_second = 5;

    log_type = LOGDEBUG;

    _filename.append(sk_conf); 
}


int sk_conf::load()
{
    string tmp;
    common_cfgparser cfg(_filename.c_str());

    cfg.get_vale("server", "ua_path", ua_path);

    cfg.get_vale("server", "id_path", id_path);
    cfg.get_vale("server", "id_num", tmp);
    id_num = atoi(tmp.c_str());

    cfg.get_vale("server", "id_financie_path", id_financie_path);

    cfg.get_vale("server", "local_strategy_path", local_strategy_path);

    cfg.get_vale("server", "http_req_thread_num", tmp);
    http_req_thread_num = atoi(tmp.c_str());

    cfg.get_vale("server", "http_server_port", tmp);
    http_server_port = atoi(tmp.c_str());

    cfg.get_vale("server", "http_server_thread_num", tmp);
    http_server_thread_num = atoi(tmp.c_str());

    cfg.get_vale("server", "ip_deny_path", ip_deny_path);

    cfg.get_vale("server", "ip_deny_num", tmp);
    ip_deny_num = atoi(tmp.c_str());

    cfg.get_vale("server", "dump_dir", dump_dir);

    cfg.get_vale("server", "reload_second", tmp);
    reload_second = atoi(tmp.c_str());

    cfg.get_vale("server", "log_type", tmp);
    int ret = atoi(tmp.c_str());

    if (ret > LOGDEBUG)
    {
        log_type = LOGDEBUG;
    }
    else if (ret <= LOGFATAL)
    {
        log_type = LOGFATAL;
    }

    return 0;
}

int sk_conf::reload()
{
    return Load();
}

bool sk_conf::need_reload()
{
    return false;
}

int sk_conf::dump()
{
    return 0;
}

int sk_conf::destroy()
{
    return 0;
}
