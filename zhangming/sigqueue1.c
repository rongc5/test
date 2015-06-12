#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>

void sigaction_handler(int s, siginfo_t *info, void *cont)
{
    if (info->si_code == SI_QUEUE)
    {
        printf("pid %d send a signal with value %d\n", info->si_pid,
                info->si_int);
    }
}
int main(void)
{
    union sigval mysigval; 

    struct sigaction act;
    act.sa_sigaction = sigaction_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO | SA_RESTART;

    pid_t pid;

    pid = fork();
    if (0 == pid)
    {
        int i = 0;
        while (1)
        {
            i++;
            mysigval.sival_int = i;
            sigqueue(getppid(), SIGUSR1, mysigval);
            sleep(1);
        }
    }

    while (1)
    {
        sigaction(SIGUSR1, &act, NULL);
    }
    return 0;
}
