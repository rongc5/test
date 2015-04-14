#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    pid_t pid;
    goto L2;
L1:
    printf("I am child!\n");
    exit(0);
L2:
    pid = fork();
    if (-1 == pid)
    {
        perror("fork");
        exit(1);
    }
    if (0 == pid)
    {
        printf("I am child my pid is %d, and my ppid is %d\n",  
                getpid(), getppid());
        goto L1;
    }else if (pid > 0)
    {
        printf("I am father and my child is %d, my pid is %d\n", pid, 
        getpid());
    }

    printf("hello world\n");
    sleep(1);
    exit(0);
}
