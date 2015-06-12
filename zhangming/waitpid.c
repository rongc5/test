#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid;
    int i, ret;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(1);
    }
    if (pid == 0)
    {
        sleep(3);
        exit(0);
    }

    while (1)
    {
        ret = waitpid(pid, NULL, WNOHANG);
        if (pid != ret){
        /*if (0 == waitpid(pid[2], NULL, 0)){*/
            printf("hello world:%d\n", ret);
            continue;
        }else if (pid == ret) 
            break;
    }

    printf("pid = %d was waited!\n", pid);

    return 0;
}
