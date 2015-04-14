#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    pid_t pid;

    printf("hello world!");

    pid = fork();
    if (-1 == pid)
    {
        perror("fork");
        exit(1);
    }

    if (0 == pid)
        exit(0);
    exit(0);
}
