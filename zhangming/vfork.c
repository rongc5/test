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

    pid = vfork();
    if (0 == pid)
    {
        glob++;
        var++;
   //     exit(0);
    }
    
    printf("pid = %d, glob = %d, var = %d\n", getpid(), glob, var);

    sleep(2);
    exit(0);
}
