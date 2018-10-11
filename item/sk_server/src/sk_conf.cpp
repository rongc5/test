#include "sk_conf.h"
#include "common_cfgparser.h"

sk_conf::sk_conf(const char * sk_conf)
{
    http_server_port = 8080;
    http_server_thread_num = 3;

    //ip_deny_num = 100;

    log_type = LOGDEBUG;

    _filename.append(sk_conf); 
    
    _strategy = NULL;

    std::string tmp;
    common_cfgparser cfg(_filename.c_str());

    cfg.get_vale("server", "strategy_path", strategy_path);
    cfg.get_vale("server", "strategy_file", strategy_file);

    cfg.get_vale("server", "http_server_port", tmp);
    http_server_port = atoi(tmp.c_str());

    cfg.get_vale("server", "http_server_thread_num", tmp);
    http_server_thread_num = atoi(tmp.c_str());

    //cfg.get_vale("server", "ip_deny_path", ip_deny_path);

    cfg.get_vale("server", "dump_dir", dump_dir);

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

    {
        strategy_conf *ua_dict1 = new (std::nothrow)strategy_conf();
        ua_dict1->init(strategy_path.c_str(), strategy_file.c_str(), dump_dir.c_str());

        strategy_conf *ua_dict2 = new (std::nothrow)strategy_conf();
        ua_dict2->init(strategy_path.c_str(), strategy_file.c_str(), dump_dir.c_str());

        _strategy = new (std::nothrow)reload_mgr<strategy_conf>(ua_dict1, ua_dict2);
    }
}


int sk_conf::load()
{
    if (_strategy)
        _strategy->reload();

    return 0;
}

int sk_conf::reload()
{
    int flag = 0;
    if (_strategy && _strategy->need_reload())
    {
        flag = 1;
        load();
    }

    return flag;
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

int sk_conf::destroy_idle()
{
    if (_strategy)
        _strategy->idle()->destroy();

    return 0;
}
