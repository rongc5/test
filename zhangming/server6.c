#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <signal.h>
#include <poll.h>
#include <errno.h>

#define SERVERADDR "192.168.1.104"
#define SERVERPORT 8888
int main(void)
{
    struct sockaddr_in serverend, clientend;
    char buf[512], ipbuf[16];
    socklen_t len;
    int sd, ret, fd, newsd;
    len = sizeof(serverend);
    pid_t pid;

    serverend.sin_family = AF_INET;
    serverend.sin_port = htons(SERVERPORT);
    inet_pton(AF_INET, SERVERADDR, &serverend.sin_addr);

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sd)
    {
        perror("socket");
        exit(1);
    }

    ret = bind(sd, (struct sockaddr *)&serverend, len);
    if (-1 == ret)
    {
        perror("bind");
        exit(1);
    }

    listen(sd, 10);

    while (1)
    {
        newsd = accept(sd, (struct sockaddr *)&clientend, &len);
        if (-1 == newsd)
        {
            perror("accept");
            exit(1);
        }
        printf("Client addr:%s, port:%d connect success!\n",
        (char *)inet_ntop(AF_INET, &clientend.sin_addr, ipbuf, 16),  ntohs(clientend.sin_port));
        pid = fork();
        if (-1 == pid)
        {
            perror("fork");
            exit(1);
        }

        if (0 == pid)
        {
            close(sd);//attention
            ret = read(newsd, buf, 512);
            if (-1 == ret)
            {
                perror("read");
                exit(1);
            }
            buf[ret] = '\0';
        }
        if (access(buf, R_OK) == -1)
        {
            printf("File %s error!\n", buf);
            sprintf(buf, "error");
            write(newsd, buf, strlen(buf));
            write(newsd, &errno, sizeof(errno));
            close(newsd);
            exit(1);
        }else
        {
            fd = open(buf, O_RDONLY);
            while (1)
            {
                ret = read(fd, buf, 30);
                if (0 == ret)
                    break;

                write(newsd, buf, ret);
                ret = rand()%5;
                if (ret == 3)
                    sleep(5);
                else
                    usleep(300000);
            }
            close(newsd);
            close(fd);
            exit(0);
        }
    }
    close(newsd);//attention!!!!!
}

