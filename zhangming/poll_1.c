#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>

int create_pipes(int (*fd)[2])
{
    int i;
    int ret;
    for (i = 0; i < 5; i++) {
        ret = pipe(fd[i]);
        if (ret == -1) {
            perror("pipe");
            return -1;
        }
    }
    return 0;
}


void child_work(int fd[5][2], int exp)
{
    int i, k = 0;
    char buf[256];

    for (i = 0; i < 5; i++) {
        if (i != exp) {
            close(fd[i][0]);
            close(fd[i][1]);
        }
    }
    close(fd[exp][0]);

    srand(getpid());
    while (1) {
        k++;
        if (exp == 0)
            sleep(30);
        else
            sleep(rand()%10 + 2);
        
        printf("pid %d write!\n", getpid());
        sprintf(buf, "My pid is %d, times=%d", getpid(), k);
        write(fd[exp][1], buf, strlen(buf));
    }
}

void father_work(int fd[5][2])
{
    int i;
    char buf[256];
    int ret;
    struct pollfd fds[5];

    for (i = 0; i < 5; i++) {
        close(fd[i][1]);
    }
    for (i = 0; i < 5; i++)
    {
        fds[i].fd = fd[i][0];
        fds[i].events = POLLIN;
    }

    while (1) {
        ret = poll(fds, 5, -1);
        if (ret == -1) {
            if (errno == EINTR)
                continue;
            else {
                perror("select");
                exit(1);
            }
        }
        if (ret == 0)
        {
            continue;
        }
        if (ret > 0) {
            for (i = 0; i < 5; i++) {
                if (fds[i].revents & POLLIN) {
                    ret = read(fd[i][0], buf, 256);
                    buf[ret] = '\0';
                    printf("%s\n", buf);
                }
            }
        }
    }
}

int main(void)
{
    int fd[5][2];
    int i, ret;
    pid_t pid;

    ret = create_pipes(fd);
    if (ret == -1)
        exit(1);

    for (i = 0; i < 5; i++) {
        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(1);
        }

        if (pid == 0) {
            child_work(fd, i);
            exit(0);
        }
    }

    father_work(fd);
    
    exit(0);
}
