#include "ua_dict.h"
#include "log_helper.h"

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

    return 0;
}
int proc_data::reload()
{

    return 0;
}
bool proc_data::need_reload()
{

    return true;
}
int proc_data::dump()
{

    return 0;
}
int proc_data::destroy()
{

    return 0;
}
