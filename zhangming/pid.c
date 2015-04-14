#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#define MAXLINE 256

int main(void)
{
    char buf[MAXLINE];
    pid_t pid;
    int status;

    printf("%% ");
    while(fgets(buf, MAXLINE, stdin) != NULL)
    {
        if (buf[strlen(buf) - 1] == '\n')
            buf[strlen(buf) - 1] = '\0';

        if ((pid = fork()) < 0)
        {
            perror("fork error");
            exit(1);
        }
        else if (0 == pid)
        {
            execlp(buf, buf, NULL);
            sprintf(buf, "%s", "hello world");
            printf("%s:", buf);
            printf("%p", buf);
            exit(127);
        }

        if ((pid = waitpid(pid, &status, 0)) < 0)
        perror("parent waitpid");
        printf("%% ");
        printf("father:%p\n", buf);
    }
    exit(0);
}
