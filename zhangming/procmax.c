#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int main(void)
{
    int i;
    struct sigaction act;
    pid_t pid;

    act.sa_handler = SIG_IGN;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_NOCLDWAIT;

    sigaction(SIGCHLD, &act, NULL);

    for (i = 0; i < 10000; i++)
    {
        pid = fork();
        if (0 == pid)
            exit(0);
        else if (-1 == pid)
        {
            perror("fork");
            exit(1);
        }
    }
    exit(0);
}
