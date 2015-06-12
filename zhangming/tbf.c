#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#define BUFSIZE 100
#define BURTSIZE 5000
#define cps 32

static int token = 0;

void alarm_handler(int s)
{
    if ((token += cps) >= 5000)
        token = 0;
    alarm(1);
}

int main(void)
{
    int fd, ret;
    char buf[BUFSIZE];

    signal(SIGALRM, alarm_handler);

    fd = open("/etc/services", O_RDONLY);
    {;}
    alarm(1);
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
