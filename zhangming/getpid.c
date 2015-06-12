#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    int a;
    printf("my pid is %d, and my parent id is %d\n", getpid(), getppid());

    while (1)
        sleep(1);

}
