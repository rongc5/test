#include <stdio.h>
#include "../src/log_helper.h"

using namespace MZFRAME;

int main(int argc, char *argv[])
{
    singleton_base<log_mgr>::set_instance(new log_mgr());
    singleton_base<log_mgr>::get_instance()->init(LOGDEBUG,"test",0, 0);

    LOG_DEBUG("hello world");

    return 0;
}
