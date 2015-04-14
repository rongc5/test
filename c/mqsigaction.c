#include <mqueue.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>


static void sig_deal(int signo, siginfo_t *info, void *context)
{
    printf("received signal %d, code %d, ival %d\n", signo, info->si_code, info->si_value.sival_int);
}
int main(int c, char **v)
{
    int i, j;
    pid_t pid;
    sigset_t newset;
    union sigval val;
    
    struct sigaction act1, act2, act3;
    act1.sa_sigaction = sig_deal;
    sigemptyset(&act1.sa_mask);
    act1.sa_flags = SA_SIGINFO | SA_RESTART;

    
    act2.sa_sigaction = sig_deal;
    sigemptyset(&act2.sa_mask);
    act2.sa_flags = SA_SIGINFO | SA_RESTART;
    
    act3.sa_sigaction = sig_deal;
    sigemptyset(&act3.sa_mask);
    act3.sa_flags = SA_SIGINFO | SA_RESTART;
    


    if ((pid = fork()) == 0) {
        sigemptyset(&newset);
        sigaddset(&newset, SIGRTMAX);
        sigaddset(&newset, SIGRTMAX - 1);
        sigaddset(&newset, SIGRTMAX - 2);
        sigprocmask(SIG_BLOCK, &newset, NULL);

        sigaction(SIGRTMAX, &act1, NULL);
            sigaction(SIGRTMAX - 1, &act2, NULL);
            sigaction(SIGRTMAX - 2, &act3, NULL);

            sleep(6);
        sigprocmask(SIG_UNBLOCK, &newset, NULL);
        sleep(3);
        exit(0);
    }

    sleep(3);
    for (i = SIGRTMAX; i >= SIGRTMAX - 2; i--) {
        for (j = 0; j <= 2; j++) {
            val.sival_int = j;
            sigqueue(pid, i, val);
            printf("sent signal %d, val = %d\n", i, j);
        }
    }
    exit(0);
}

