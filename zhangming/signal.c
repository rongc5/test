#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void signal_handler(int s)
{
    printf("   %d was catched!\n", s);
    //sleep(10);
    //printf("over!\n");
    exit(0);
}

int main(void)
{
    signal(SIGINT, signal_handler);
    signal(SIGQUIT, signal_handler);
    while(1)
    {
        printf("hello world!\n");
        sleep(1);
    }
}
