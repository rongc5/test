#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define SOCKETPATH "/home/rong/project/c/千锋C语言老师邮箱"

int main(void)
{
    int sd, ret, newsd;
    struct sockaddr_un serverend;
    socklen_t len;
    char buf[512];
    pid_t pid;

    len = sizeof(serverend);
    serverend.sun_family = AF_UNIX;
    strncpy(serverend.sun_path, SOCKETPATH, 108);

    daemon(1, 1);
    sd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sd == -1)
    {
        perror("socket");
        exit(1);
    }

    unlink(SOCKETPATH);
    ret = bind(sd, (struct sockaddr *)&serverend, len);
    if (-1 == ret)
    {
        perror("bind");
        exit(1);
    }

    listen(sd, 10);

    while (1)
    {
        newsd = accept(sd, NULL, NULL);
        pid = fork();
        if (pid == 0)
        {
            ret = read(newsd, buf, 512);
            if (-1 == ret)
            {
                perror("read");
                exit(1);
            }
            if (ret == 0)
            {
                fprintf(stderr, "end of file!\n");
                exit(0);
            }
            else if (ret > 0)
            {
                buf[ret] = '\0';
                printf("From client:%s\n", buf);
            }
            exit(0);
        }
        else if (pid > 0)
            {
                sleep(1);
                write(newsd, "hello world", 11);

            }
        wait(NULL);
        }
    }

