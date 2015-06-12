#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void sigaction_handler(int s, siginfo_t *info, void* cont)
{
    if (info->si_code == SI_QUEUE)
    {
        printf("pid %d send a signal with value %d!\n", info->si_pid, 
                info->si_int);
    }
}

int main(void)
{
    int ret;
    struct sigaction act;

    act.sa_sigaction = sigaction_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO | SA_RESTART;

    sigaction(SIGUSR1, &act, NULL);

    while (1)
        sleep(1); 
    exit(0);
}
