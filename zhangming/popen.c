#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int c, char **v)
{
    int i;

    for (i = 1; i < c; i++)
    {
        if (strcmp(v[i], "qq") == 0)
        {
            v[i] = NULL;
            break;
        }
    }

    if (i == c || i == c-1)
    {
        fprintf(stderr, "usage...\n");
        exit(1);
    }

    int fd[2];
    pid_t pid;
    int ret;

    ret = pipe(fd);
    if (ret == -1)
    {
        perror("pipe");
        exit(1);
    }

    pid = fork();
    if (-1 == pid)
    {
        perror("fork");
        exit(1);
    }

    if (0 == pid)
    {
        close(fd[0]);
        printf("%s\n", v[1]);
        dup2(fd[1], 1);
        ret = execvp(v[1], &v[1]);
        if (-1 == ret)
        {
            perror("execvp");
            exit(1);
        }
        exit(1);
    }

    close(fd[1]);
    wait(NULL);
    printf("%s\n", v[i+1]);
    dup2(fd[0], 0);
    ret = execvp(v[i+1], v+i+1);
    if (-1 == ret)
    {
        perror("execvp");
        exit(1);
    }
}
