#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#define BUFSIZE 100

static int times = 0;

void alarm_handler(int s)
{
    times++;
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
        while (0 == times)
            pause();
        ret = read(fd, buf, BUFSIZE);
        if (0 == ret)
            break;
        write(1, buf, ret);
        times;
    }

    close(fd);

    exit(0);
}
