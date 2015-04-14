#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

int ret = 0;
void sig_alrm(int sigo);
int main(void)
{
    int fd[2];
   // int ret;
    pid_t pid;

    signal(SIGALRM, sig_alrm);
    ret = pipe(fd);
    if (-1 == ret)
    {
        perror("pipe");
        exit(1);
    }

   // ret = 0;
//    close(fd[0]);
    while (1)
    {
        write(fd[1], "b", 1);
            ret++;
    //    printf("%d\n", ret);
//        alarm(1);
    }
    alarm(1);

    printf("%d\n", ret);

    return 0;
}

void sig_alrm(int sigo)
{
    printf("%d\n", ret);

    exit(0);
}
