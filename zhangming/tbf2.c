#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/time.h>

#define BUFSIZE 100
#define BURTSIZE 5000
#define cps 2

static int token = 0;

void alarm_handler(int s)
{
    if ((token += cps) >= 5000)
        token = 0;
 //   alarm(1);
}

int main(void)
{
    int fd, ret;
    char buf[BUFSIZE];
    struct itimerval cur;
    signal(SIGALRM, alarm_handler);

    fd = open("/etc/services", O_RDONLY);
    {;}

    cur.it_interval.tv_sec = 0;
    cur.it_interval.tv_usec = 50000;

    cur.it_value.tv_sec = 5;
    cur.it_value.tv_usec = 50000;

    setitimer(ITIMER_REAL, &cur, NULL);

    while (1)
    {
        while (0 == token)
            pause();
        ret = read(fd, buf, cps);
        if (0 == ret)
            break;
        write(1, buf, ret);
        token -= ret;
    }

    close(fd);

    exit(0);
}
