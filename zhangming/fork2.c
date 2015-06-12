#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int glob = 6;
char buf[] = "a write to stdout\n";

int main(void)
{
    int var = 88;
    pid_t pid;
    write(1, buf, sizeof(buf) - 1);

    pid = fork();
    if (0 == pid)
    {
        glob++;
        var++;
    }
    else
        sleep(2);

    printf("pid = %d, glob = %d, var = %d\n", getpid(), glob, var);

    return 0;
}
