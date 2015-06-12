#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

void display(struct timeval *prev, struct timeval *next)
{
    suseconds_t usec;
    time_t sec;
    unsigned long msec;

    sec = next->tv_sec - prev->tv_sec;
    usec = next->tv_usec - prev->tv_usec;

    usec += sec * 1000000;
    msec = usec / 1000;
    printf("%lu was consumed\n", msec);

}

int main(void)
{
    unsigned long long i;
    struct timeval prev, next;

    gettimeofday(&prev, NULL);
    for (i = 0; i < 10000000; i++)
    {
        i++;
        i--;
    }
    gettimeofday(&next, NULL);

    display(&prev, &next);

    return 0;
}
