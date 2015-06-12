#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int g_val = 0;

int main(void)
{
    pid_t pid;

    pid = fork();
    if (-1 == pid)
    {
        perror("fork");
        exit(1);
    }
    if (0 == pid)
    {
        g_val = 9;
        exit(0);
    }
    sleep(1);
    printf("g_val = %d\n", g_val);

    exit(1);
}
