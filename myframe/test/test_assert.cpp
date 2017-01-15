#include "../src/base_def.h"
#include "../src/log_helper.h"

#include <stdio.h>

int main(int argc, char *argv[])
{

    ASSERT_DO(ret != -1, (LOG_WARNING("socketpair fail errstr[%s]", strerror(errno)))); 

    return 0;
}

