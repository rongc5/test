#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <fcntl.h>
#include <signal.h>
#include <poll.h>
#include <errno.h>

#define SERVERADDR "192.168.1.146"
#define SERVERPORT 8080


#define FRAMESIZE RATE*FMTS*(CHANNELS+1)/8

int main(void)
{
    struct sockaddr_in serverend, clientend;
    socklen_t len;
    int ret, sd, fd, newsd;
    char buf[FRAMESIZE];
    char ipbuf[16];
    pid_t pid;

    len = sizeof(serverend);
    serverend.sin_family = AF_INET;
    serverend.sin_port = htons(SERVERPORT);
    inet_pton(AF_INET, SERVERADDR, &serverend.sin_addr);

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sd)
    {
        perror("socket");
        exit(1);
    }

    listen(sd, 10);

    while (1) {
        newsd = accept(sd, (struct sockaddr *)&clientend, &len);
        if (newsd == -1) {
            perror("accept");
            exit(1);
        }

        printf("client addr:%s, port:%d connect success!\n", (char *)inet_ntop(AF_INET, &clientend.sin_addr, ipbuf, 16), ntohs(clientend.sin_port));
        if (pid == 0)
        {
            close(sd);
            ret = read(newsd, buf, 256);
            if (-1 == ret)
            {
                perror("read");
                exit(1);
            }
            buf[ret] = '\0';
            if (access(buf, R_OK) == -1)
            {
                printf("file %s error!\n", buf);
                sprintf(buf, "error");
                write(newsd, buf, strlen(buf));
                write(newsd, &errno, sizeof(errno));
                close(newsd);
                exit(1);
            }
            else
            {
                fd = open(buf, O_RDONLY);
                while (1)
                {
                    ret = read(fd, buf, FRAMESIZE);
                    if (0 == ret)
                        break;

                    write(newsd, buf, ret);
                   // ret = rand()%5;
                   // if (ret == 3)
                     //   sleep(5);
                   // else
                        usleep(300000);
                }
                close(newsd);
                close(fd);
                exit(0);
            }
        }

        close(newsd);
    }
}
