#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <string.h>

#define SERVERPATH "/tmp/serverend"

int main(void)
{
    int sd, ret, newsd;
    struct sockaddr_un serverend;
    socklen_t len;
    char buf[512];
    pid_t pid;

    len = sizeof(serverend);
    serverend.sun_family = AF_UNIX;
    strcpy(serverend.sun_path, SERVERPATH);

    daemon(1, 1);
    sd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sd == -1)
    {
        perror("socket");
        exit(1);
    }

    unlink(SERVERPATH);
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
        if (-1 == pid)
        {
            perror("fork");
            exit(1);
        }
        if (pid > 0)
        {
            close(newsd);
            continue;
        }
        close(sd);
        ret = read(newsd, buf, 512);
        if (ret == -1)
        {
            perror("read");
            exit(1);
        }
        if (0 == ret)
        {
            fprintf(stderr, "end of file!\n");
            exit(0);
        }else if (ret > 0)
        {
            buf[ret] = '\0';
            printf("From client:%s\n", buf);
            write(newsd, "hello qianfeng", 23);
        }
        exit(0);
    }
}
