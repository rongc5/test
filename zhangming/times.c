#include <stdio.h>
#include <sys/times.h>
#include <unistd.h>

int main(void)
{
    struct tms p, n;
    clock_t a, b;
    int i = 0;

    double tic_sec;
    a = times(&p);
    while (1)
        if (i++ == 100000000)
            break;
    b = times(&n);

    tic_sec = (double) 1/ (double)sysconf(_SC_CLK_TCK);
    printf("cosume wall time: %f\n", (double)(b - a) * tic_sec);
    printf("user:%f\n", (double)(n.tms_utime - p.tms_utime) * tic_sec);
    
    return 0;
}
