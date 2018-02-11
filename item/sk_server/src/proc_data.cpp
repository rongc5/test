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
