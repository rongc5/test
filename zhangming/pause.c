#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void alarm_handler(int s)
{
    return;
}

void mysleep(int s)
{
    int ret;
    signal(SIGALRM, alarm_handler);
    alarm(s);
    pause();
}

int main(void)
{
    signal(SIGINT, alarm_handler);
    printf("I will sleep!\n");
    mysleep(5);
    printf("wakeup!\n");
    return 0;
}
