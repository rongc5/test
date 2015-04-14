#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>

#define FMT_EOT "\r\n"

void music_rep(int sd)
{
    static int cnt = 1;

    char buf[4096];
    int ret, fd;
    char *path = "htys.mp3";
    fd = open(path, O_RDONLY);
    if (-1 == fd)
    {
        perror("open");
        exit(1);
    }

    printf("send file %s\n", path);

    while (1)
    {
        ret = read(fd, buf, 4096);
        if (0 == ret)
        {
            break;
        }
        srand(getpid());
        usleep(rand()%500000);
        write(sd, buf, ret);
    }
    close(fd);
}

int main(void)
{
    int sd, newsd, ret, opt;
    pid_t pid;
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr);

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sd)
    {
        perror("socket");
        exit(1);
    }

    opt = 1;
    ret = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));
    if (-1 == ret)
    {
        perror("setsockopt");
        exit(1);
    }

    ret = bind(sd, (struct sockaddr *)&addr, addrlen);
    if (-1 == ret)
    {
        perror("bind");
        exit(1);
    }

    ret = listen(sd, 10);
    if (-1 == ret)
    {
        perror("listen");
        exit(1);
    }

    while (1)
    {
        newsd = accept(sd, NULL, NULL);
        if (-1 == newsd)
        {
            perror("accept");
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
            close(sd);
            music_rep(newsd);
            close(newsd);
            exit(0);
        }
        close(newsd);
    }
}
