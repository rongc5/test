#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    pid_t pid;
    int i;
    for (i = 0; i < 10000; i++)
    {
        pid = fork();
        if (pid == 0)
        {
            exit(0);
        }

        waitpid(pid, NULL);
    }

    return 0;
}
