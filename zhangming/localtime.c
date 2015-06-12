#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
    time_t cur;
    struct tm * res;

    time(&cur);
    res = localtime(&cur);
    if (NULL == res)
    {
        perror("localtime");
        exit(1);
    }

    printf("%d-%d-%d %d:%d:%d\n", res->tm_year + 1900, res->tm_mon + 1,
            res->tm_mday, res->tm_hour, res->tm_min, res->tm_sec);

    return 0;
}
