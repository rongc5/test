#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    struct rlimit rlim;
    int ret;
    ret = getrlimit(RLIMIT_DATA, &rlim);

    if (ret)
    {
        perror("getrlimit");
        exit(1);
    }

    printf("soft:%u\n", (unsigned int)rlim.rlim_cur);
    printf("hard:%u\n", (unsigned int)rlim.rlim_max);

    return 0;
}
