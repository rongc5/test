#include "sk_conf.h"
#include "common_cfgparser.h"

sk_conf::sk_conf(const char * sk_conf)
{
    http_server_port = 8080;
    http_server_thread_num = 3;

    //ip_deny_num = 100;

    reload_second = 5;

    req_interval_second = 20;
    req_http_timeout = 300;
    max_reqhttp_num = 10;

    log_type = LOGDEBUG;

    _filename.append(sk_conf); 
}


int sk_conf::load()
{
    std::string tmp;
    common_cfgparser cfg(_filename.c_str());

    cfg.get_vale("server", "ua_path", ua_path);
    cfg.get_vale("server", "ua_file", ua_file);

    cfg.get_vale("server", "id_path", id_path);
    cfg.get_vale("server", "id_file", id_file);

    cfg.get_vale("server", "financie_path", financie_path);
    cfg.get_vale("server", "financie_file", financie_file);

    cfg.get_vale("server", "ban_path", ban_path);
    cfg.get_vale("server", "ban_file", ban_file);

    cfg.get_vale("server", "local_strategy_path", local_strategy_path);
    cfg.get_vale("server", "local_strategy_file", local_strategy_file);

    cfg.get_vale("server", "plate_dict_path", plate_dict_path);
    cfg.get_vale("server", "plate_dict_file", plate_dict_file);

    cfg.get_vale("server", "address_dict_path", address_dict_path);
    cfg.get_vale("server", "address_dict_file", address_dict_file);

    cfg.get_vale("server", "history_single_path", history_single_path);
    cfg.get_vale("server", "history_single_file", history_single_file);

    cfg.get_vale("server", "history_single_day", tmp);
    history_single_day = atoi(tmp.c_str());

    cfg.get_vale("server", "history_quotation_path", history_quotation_path);
    cfg.get_vale("server", "history_quotation_file", history_quotation_file);

    cfg.get_vale("server", "history_quotation_day", tmp);
    history_quotation_day = atoi(tmp.c_str());

    cfg.get_vale("server", "http_server_port", tmp);
    http_server_port = atoi(tmp.c_str());

    cfg.get_vale("server", "http_server_thread_num", tmp);
    http_server_thread_num = atoi(tmp.c_str());

    //cfg.get_vale("server", "ip_deny_path", ip_deny_path);

    cfg.get_vale("server", "dump_dir", dump_dir);

    cfg.get_vale("server", "reload_second", tmp);
    reload_second = atoi(tmp.c_str());

    cfg.get_vale("server", "req_interval_second", tmp);
    req_interval_second = atoi(tmp.c_str());


    cfg.get_vale("server", "req_http_timeout", tmp);
    req_http_timeout = atoi(tmp.c_str());

    cfg.get_vale("server", "max_reqhttp_num", tmp);
    max_reqhttp_num = atoi(tmp.c_str());

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
    else 
    {
        log_type = (LogType) ret;
    }
        

    cfg.get_vale("server", "real_morning_stime", real_morning_stime);
    cfg.get_vale("server", "real_morning_etime", real_morning_etime);

    cfg.get_vale("server", "real_afternoon_stime", real_afternoon_stime);
    cfg.get_vale("server", "real_afternoon_stime", real_afternoon_stime);

    return 0;
}

int sk_conf::reload()
{
    return load();
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
