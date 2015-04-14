#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void sig_handler(int s)
{
    printf("catch a signal!\n");
//    exit(0);
}

int main(void)
{
    pid_t pid;

    signal(SIGCHLD, sig_handler);
    signal(SIGALRM, sig_handler);
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(1);
    }

    if (0 == pid)
    {
        alarm(3);
        while (1)
        {
            sleep(1);
        }
    }

    wait(NULL);
    exit(0);
}
