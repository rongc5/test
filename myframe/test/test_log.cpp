#include <stdio.h>
#include "../core/log_helper.h"


int main(int argc, char *argv[])
{
     log_conf conf;

     LOG_INIT(conf);

    LOG_DEBUG("hello world");
    LOG_NOTICE("hello world");
    LOG_NOTICE("hello world");
    LOG_WARNING("hello world %d", 1);
    LOG_WARNING("hello world");

    LOG_WARNING("socketpair fail errstr[%s]", "hello world test");

    return 0;
}
