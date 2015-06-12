#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

void fun1(void)
{
    int fd[2];
    int ret;
//    dup2(fd[0], 0);
  //  dup2(fd[1], 1);
    char *v[2];
    pid_t pid;
    ret = pipe(fd);
    if (-1 == ret)
    {
        perror("pipe");
        exit(1);
    }
    pid = fork();
    if (0 == pid)
    {
  //      dup2(fd[0], 0);
        close(fd[1]);
       // execlp(ls, ls, -al, NULL);
        scanf("%s %s", v[0], v[1]);
        fflush(stdin);

        printf("\n%s\n%s\n", v[0], v[1]);
        close(fd[0]);
//        exit(0);
    }
    else
    {
        waitpid(pid, NULL, 0);
    //    dup2(fd[1], 1);
        close(fd[0]);
        execlp("v[0]", "v[0]", "v[1]", NULL);
        close(fd[1]);
    }

}
int main(void)
{
    fun1();

    return 0;

}
