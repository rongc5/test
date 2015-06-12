#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>

int creat_pipes(int (*fd)[2])
{
    int i;
    int ret;
    for (i = 0; i < 5; i++)
    {
        ret = pipe(fd[i]);
        {;}
    }

    return 0;
}

void child_work(int fd[5][2], int exp)
{
    int i, k = 0;
    char buf[256];

    for (i = 0; i < 5; i++)
    {
        if (i != exp)
        {
            close(fd[i][0]);
            close(fd[i][0]);
        }
    }
    close(fd[exp][0]);

    srand(getpid());
    while (1)
    {
        k++;
        sleep(rand()%10 + 2);
        printf("pid %d write!\n", getpid());
        sprintf(buf, "my pid is %d, times = %d\n", getpid(), k);
        write(fd[exp][1], buf, strlen(buf));
    }
}

void father_work(int fd[5][2])
{
    int i;
    char buf[256];
    int ret;

    for (i = 0; i < 5; i++)
    {
        close(fd[i][1]);
        ret = fcntl(fd[i][0], F_GETFL);
    }

    while (1)
    {
        for (i = 0; i < 5; i++)
        {
            ret = read(fd[i][0], buf, 256);
            if (-1 == ret)
            {
                if (errno == EINTR || errno == EAGAIN)
                    continue;
            }
            buf[ret] = '\0';
            printf("%s\n", buf);
        }
    }
}

int main(void)
{
    int fd[5][2];
    int i, ret;
    pid_t pid;

    ret = creat_pipes(fd);
    {;}

    for (i = 0; i < 5; i++)
    {
        pid = fork();
        if (-1 == pid)
        {
            perror("fork");
            exit(1);
        }

        if (0 == pid)
        {
            child_work(fd, i);
            exit(0);
        }
    }

    father_work(fd);

    exit(0);
}

