#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


void alarm_handler(int s)
{
    printf("hello world!\n");
}

int main(void)
{
    int ret;
    signal(SIGALRM, alarm_handler);
    alarm(100);
    sleep(3);
    ret = alarm(5);
    printf("ret = %d\n", ret);
    pause();
    
    return 0;
}
