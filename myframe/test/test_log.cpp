#include <stdio.h>
#include "../src/log_helper.h"
#include "../src/base_singleton.h"
#include "../src/base_def.h"

using namespace MZFRAME;

int main(int argc, char *argv[])
{
    base_singleton<log_mgr>::set_instance(new log_mgr());
    base_singleton<log_mgr>::get_instance()->init(LOGDEBUG,"test",0, 1);

    LOG_DEBUG("hello world");
    LOG_NOTICE("hello world");
    LOG_NOTICE("hello world");
    LOG_WARNING("hello world %d", 1);
    LOG_WARNING("hello world");

    ASSERT_DO(ret != -1, (LOG_WARNING("socketpair fail errstr[%s]", strerror(errno))));

    return 0;
}
