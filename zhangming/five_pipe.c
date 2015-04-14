#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
    pid_t pid[5];
    int i = 0;
    int fd[5][2];
    int ret;
    char buf[10];
    int k = 5;

    for (i; i < 5; i++)
    {
        ret = pipe(fd[i]);
        if (-1 == ret)
        {
            perror("pipe");
            exit(1);
        }
        pid[i] = fork();

        if (pid[i] == 0)
        {
            close(fd[i][0]);
            srand(getpid());
  //          sleep(rand()%3);
            write(fd[i][1], "A", 1);
            close(fd[i][1]);
            exit(0);
        }
    }
    i = 0;
//    wait(NULL);
    while (1)
    {
        for (k = 0; k < 5; k++)
        {
            close (fd[k][1]);
            ret = read(fd[k][0], buf, 10);
            if (ret = -1)
                continue;
            else
            {
                buf[ret] = '\0';
                printf("%s\n", buf);
                i++;
                close(fd[k][0]);
            }
            if (i == 5)
                break;
        }
    }

    exit(0);
}
