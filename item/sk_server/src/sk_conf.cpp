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
    cfg.get_vale("server", "ua_file", ua_file);

    cfg.get_vale("server", "id_path", id_path);
    cfg.get_vale("server", "id_file", id_file);
    cfg.get_vale("server", "id_num", tmp);
    id_num = atoi(tmp.c_str());

    cfg.get_vale("server", "financie_path", financie_path);
    cfg.get_vale("server", "financie_file", financie_file);
    cfg.get_vale("server", "financie_num", tmp);
    financie_num = atoi(tmp.c_str());

    cfg.get_vale("server", "ban_path", ban_path);
    cfg.get_vale("server", "ban_file", ban_file);
    cfg.get_vale("server", "ban_num", tmp);
    ban_num = atoi(tmp.c_str());

    cfg.get_vale("server", "local_strategy_path", local_strategy_path);
    cfg.get_vale("server", "local_strategy_file", local_strategy_file);

    cfg.get_vale("server", "history_single_path", history_single_path);
    cfg.get_vale("server", "history_single_file", history_single_file);
    cfg.get_vale("server", "history_single_num", tmp);
    history_single_num = atoi(tmp.c_str());

    cfg.get_vale("server", "history_quotation_path", history_quotation_path);
    cfg.get_vale("server", "history_quotation_file", history_quotation_file);
    cfg.get_vale("server", "history_quotation_num", tmp);
    history_quotation_num = atoi(tmp.c_str());

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

    cfg.get_vale("server", "real_morning_stime", real_morning_stime);
    cfg.get_vale("server", "real_morning_etime", real_morning_etime);

    cfg.get_vale("server", "real_afternoon_stime", real_afternoon_stime);
    cfg.get_vale("server", "real_afternoon_stime", real_afternoon_stime);

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
