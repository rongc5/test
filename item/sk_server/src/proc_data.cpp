#include "ua_dict.h"
#include "strategy_dict.h"
#include "log_helper.h"

int Proc_data::init(sk_conf * conf)
{
    _conf = conf;
    
    ua_dict *ua_dict1 = new (std::nothrow)ua_dict();
    ASSERT_WARNING(ua_dict1 != NULL, "allocate ua_dict fail");
    ua_dict1->init(_conf->ua_path.c_str());
    
    ua_dict *ua_dict2 = new (std::nothrow)ua_dict();
    ASSERT_WARNING(ua_dict2 != NULL, "allocate ua_dict fail");
    ua_dict2->init(_conf->ua_path.c_str());

    _ua_dict = new (std::nothrow)reload_mgr<ua_dict>(ua_dict1, ua_dict2);

    strategy_dict *strategy_dict1 = new (std::nothrow)strategy_dict();
    ASSERT_WARNING(strategy_dict1 != NULL, "allocate strategy_dict fail");
    strategy_dict1->init(_conf->local_strategy_path.c_str());
    
    strategy_dict *strategy_dict2 = new (std::nothrow)strategy_dict();
    ASSERT_WARNING(strategy_dict2 != NULL, "allocate strategy_dict fail");
    strategy_dict2->init(_conf->local_strategy_path.c_str());

    ua_dict *id_dict1 = new (std::nothrow)ua_dict();
    ASSERT_WARNING(id_dict1 != NULL, "allocate id_dict fail");
    id_dict1->init(_conf->id_path.c_str());
    
    ud_dict *id_dict2 = new (std::nothrow)ua_dict();
    ASSERT_WARNING(id_dict2 != NULL, "allocate id_dict fail");
    id_dict2->init(_conf->id_path.c_str());

    _id_dict = new (std::nothrow)reload_mgr<ua_dict>(id_dict1, id_dict2);
}

proc_data * proc_data::instance()
{
    if(!_singleton) 
    {
        _singleton = new(std::nothrow) Proc_data();
        if( singleton == NULL)
        {
            LOG_WARNING("new Proc_data fail");
            exit(1);
        }
    }

    return _singleton;
}

int proc_data::load()
{
    _ua_dict->load();

    _id_dict->load();

    _strategy_dict->load();

    return 0;
}
int proc_data::reload()
{
    if (_ua_dict->need_reload())
    {
        _ua_dict->reload();
    }

    if (_id_dict->need_reload())
    {
        _id_dict->reload();
    }

    if (_strategy_dict->need_reload())
    {
        _strategy_dict->reload();
    }

    return 0;
}
bool proc_data::need_reload()
{
    return true;
}
int proc_data::dump()
{
    _ua_dict->dump();

    _id_dict->dump();

    _strategy_dict->dump();

    return 0;
}
int proc_data::destroy()
{
    _ua_dict->destroy();

    _id_dict->destroy();

    _strategy_dict->destroy();

    return 0;
}
