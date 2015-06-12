#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
    time_t cur;
    struct tm * res;
    char buf[256];

    time(&cur);
    res = localtime(&cur);
    if (NULL == res)
    {
        perror("localtime");
        exit(1);
    }
    
    //strftime(buf, 256, "%x-%X", res);
    strftime(buf, 256, "%Y-%m-%d %H:%M:%S", res);

    printf("%s\n", buf);

    return 0;
}
