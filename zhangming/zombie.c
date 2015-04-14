#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    pid_t pid;
    int i;
    for (i = 0; i < 3; i++)
    {
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(1);
        }

        if (pid == 0)
        {
            srand(getpid());
            sleep(rand()%5 + 1);
            exit(0);
        }
        printf("pid %d was created!\n", pid);

    }
    for (i = 0; i < 3; i++)
    {
        pid = wait(NULL);
        printf("pid %d was over!\n", pid);
    }           

    while (1)
        sleep(1);
}

