#include "ua_dict.h"
#include "log_helper.h"

int Proc_data::init(sk_conf * conf)
{
    _conf = conf;
    
    _ua_dict = new (std::nothrow)ua_dict();
    ASSERT_WARNING(_ua_dict != NULL, "allocate ua_dict fail");
    _ua_dict->init(_conf->ua_path.c_str(), _conf->ua_num);

    


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

    return 0;
}
int proc_data::reload()
{
    if (_ua_dict->need_reload())
    {
        _ua_dict->reload();
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

    return 0;
}
int proc_data::destroy()
{
    _ua_dict->destroy();

    return 0;
}
