#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    printf("pid = %d\n", getpid());
    daemon(1, 1);
    printf("pid = %d\n", getpid());
    while (1)
    {
        printf("daemon!\n");
        sleep(1);
    }
}
