#include <signal.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int fg = 0;

void async_read(int s) 
{
    char buf[256];
    int ret;

    ret = read(0, buf, 256);
    buf[ret] = '\0';
    printf("fg = %d, read:%s", fg, buf);    
}

int main(void)
{
    struct sigaction act;
    int ret;

    act.sa_handler = async_read;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_RESTART;

    sigaction(SIGIO, &act, NULL);

    ret = fcntl(0, F_GETFL);
    fcntl(0, F_SETFL, ret | O_ASYNC);

    fcntl(0, F_SETOWN, getpid());

    while (1)
    {
        sleep(1);
        fg++;
        printf("I am running!\n");
    }
}

